#include "../include/server.h"
#include <ctime>
#include <unistd.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include "server.h"

Server::Server(LibraryManager *libraryManager) : serverSocket(-1), libraryManager(*libraryManager) {}

Server::~Server()
{
    if (serverSocket != -1)
    {
        close(serverSocket);
    }
}

// Kill the process using the port to free it
void Server::killProcessUsingPort()
{
    // Construct the command to find the PID using lsof and kill it
    std::stringstream command;
    command << "lsof -t -i:" << port; // Get the PID of the process using the port

    // Execute the command and capture the PID
    FILE *fp = popen(command.str().c_str(), "r");
    if (fp == nullptr)
    {
        std::cerr << "Failed to execute lsof command\n";
        return;
    }
    std::cout << "Killing; " << command.str() << std::endl;

    char pid[10];
    if (fgets(pid, sizeof(pid), fp) != nullptr)
    {
        // Kill the process by sending SIGKILL to the PID
        int pidInt = atoi(pid);
        std::cout << "Killing process with PID: " << pidInt << std::endl;
        kill(pidInt, SIGKILL);
    }

    pclose(fp);
}

void Server::start()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        std::cerr << "Failed to create socket\n";
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Bind failed, attempting to kill process occupying the port...\n";

        // Kill the process occupying the port
        killProcessUsingPort();

        // Retry binding after killing the process
        if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            std::cerr << "Bind failed again after killing the process\n";
            return;
        }

        std::cout << "Successfully bound to the port after killing the process\n";
    }

    if (listen(serverSocket, 5) < 0)
    {
        std::cerr << "Listen failed\n";
        return;
    }

    std::cout << "Server is running on port " << port << "\n";

    while (true)
    {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
        if (clientSocket < 0)
        {
            std::cerr << "Failed to accept connection\n";
            continue;
        }

        clientThreads.emplace_back(&Server::handleClient, this, clientSocket, std::ref(libraryManager));
        cout << "New Client connected\n";
    }

    for (auto &thread : clientThreads)
    {
        thread.join();
    }
}

void Server::handleClient(int clientSocket, LibraryManager &libraryManager)
{
    Library library = libraryManager.getLibrary();

    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0)
            break;

        std::string request(buffer);
        std::string response;
        cout << "Request: " << request << std::endl;
        size_t pos1 = request.find('|');
        size_t pos2 = request.find('|', pos1 + 1);
        if (pos1 == std::string::npos || pos2 == std::string::npos)
        {
            response = "Invalid request format\n";
        }
        else
        {
            std::string operation = request.substr(0, pos1);
            int userId = std::stoi(request.substr(pos1 + 1, pos2 - pos1 - 1));
            int bookId = std::stoi(request.substr(pos2 + 1));
            cout << "Operation: " << operation << ", User ID: " << userId << ", Book ID: " << bookId << std::endl;
            if (operation == "borrow")
            {
                std::time_t t = std::time(nullptr);
                std::tm currentDate;
                currentDate = *localtime(&t);

                // Calculate the future date
                std::tm dueDateTm = libraryManager.calculateFutureDate(currentDate, 14);

                // Format the future date as a string
                std::stringstream ss;
                ss << std::put_time(&dueDateTm, "%Y-%m-%d");
                std::string dueDate = ss.str();
                if (library.borrowBook(userId, bookId, dueDate))
                {
                    libraryManager.logTransaction(Transaction(static_cast<int>(libraryManager.getTransactionCount()) + 1, userId, bookId, "Book Borrowed"));
                    response = "\nBook borrowed successfully. Due date: " + dueDate + "\n";
                }
                else
                {
                    response = "\nFailed to borrow book.\n";
                }
            }
            else if (operation == "return")
            {
                if (library.returnBook(userId, bookId))
                {
                    libraryManager.logTransaction(Transaction(static_cast<int>(libraryManager.getTransactionCount()) + 1, userId, bookId, "Book Returned"));
                    response = "\nBook returned successfully.\n";
                }
                else
                {
                    response = "\nInvalid Book OR User Id\n";
                }
            }
            else if (operation == "all_books")
            {
                for (const auto &book : library.getBooks())
                {
                    response += book->display();
                }
                if (response.empty())
                {
                    response = "\nNo books available.\n";
                }
            }
            else if (operation == "book_borrow")
            {
                auto user = library.searchUser(userId);
                if (user)
                {
                    if (user->getBorrowedBooks().empty())
                    {
                        response = "\nUserId not exist.";
                    }
                    else
                    {
                        for (const auto &borrowedBook : user->getBorrowedBooks())
                        {
                            auto book = library.searchBook(borrowedBook.book->getId());
                            if (book)
                            {
                                response += "\n Book ID: " + std::to_string(book->getId()) + ", DueDate: " + borrowedBook.dueDate + "\n";
                            }
                        }
                    }
                }
                else
                {
                    response = "\nUserId not exist.";
                }
            }
            else
            {
                response = "Invalid request......1.1.1.\n";
            }
        }
        cout << "Saving data: " << std::endl;
        libraryManager.saveData();
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close(clientSocket);
}
