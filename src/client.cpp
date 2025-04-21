#include "../include/client.h"
#include <unistd.h>

Client::Client() : clientSocket(-1) {}

Client::~Client()
{
    if (clientSocket != -1)
    {
        close(clientSocket);
    }
}

void Client::start()
{
    std::string serverIP = "127.0.0.1";   //localhost
    int port = 8080;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "Failed to create socket\n";
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address\n";
        return;
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Connection failed\n";
        return;
    }

    std::cout << "Connected to server\n";

    while (true)
    {
        int choice;
        std::cout << "1. Borrow Book\n2. Return Book\n3. Display All Books\n4. Display Borrowed Books By User\n5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 5)
            break;

        int userId, bookId;
        std::string message;

        switch (choice)
        {
        case 1: // Borrow Book
            std::cout << "Enter User ID: ";
            std::cin >> userId;
            std::cout << "Enter Book ID: ";
            std::cin >> bookId;
            message = "borrow|" + std::to_string(userId) + "|" + std::to_string(bookId);
            break;

        case 2: // Return Book
            std::cout << "Enter User ID: ";
            std::cin >> userId;
            std::cout << "Enter Book ID: ";
            std::cin >> bookId;
            message = "return|" + std::to_string(userId) + "|" + std::to_string(bookId);
            break;
        case 3: // Display All Books
            message = "all_books|" + std::to_string(0) + "|" + std::to_string(0);
            break;
        case 4: // Display All Books Borrowed By User
            int userId;
            std::cout << "Enter User ID: ";
            std::cin >> userId;
            std::cin.ignore();
            message = "book_borrow|" + std::to_string(userId) + "|" + std::to_string(0);
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
            continue;
        }

        send(clientSocket, message.c_str(), message.size(), 0);

        char buffer[1024] = {0};
        // int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        // std::cout << "Server: " << buffer;
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0)
        {
            std::cout << "Server: " << std::string(buffer, bytesRead) << std::endl;
        }
        else if (bytesRead == 0)
        {
            std::cout << "Server closed the connection." << std::endl;
            break;
        }
        else
        {
            std::cerr << "Error receiving data from server." << std::endl;
            break;
        }
    }
}
