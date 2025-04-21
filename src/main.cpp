
#include <iostream>
#include <string>
#include "../include/libraryManager.h"
#include "../include/server.h"
#include "../include/client.h"
#include <csignal>

Server *serverInstance = nullptr;
LibraryManager *libraryManager = nullptr;

// Signal handler for SIGINT (Ctrl+C)
void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    if (libraryManager)
    {
        libraryManager->saveData();
        delete libraryManager;
        libraryManager = nullptr;
    }
    if (serverInstance)
    {
        serverInstance->killProcessUsingPort();
        delete serverInstance;
        serverInstance = nullptr;
    }
    exit(signum);
}

// Run the console application
void runConsoleApp()
{
    libraryManager = new LibraryManager();
    libraryManager->start();
}

// Run the server
void runServer()
{
    libraryManager = new LibraryManager();
    libraryManager->loadData();
    serverInstance = new Server(libraryManager);
    serverInstance->start();
}

// Run the client
void runClient()
{
    Client client;
    client.start();
}

int main()
{

    signal(SIGINT, signalHandler);

    int choice;

    std::cout << "Select Mode:\n";
    std::cout << "1. Run Console Application\n";
    std::cout << "2. Run as Server\n";
    std::cout << "3. Run as Client\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        runConsoleApp();
        break;
    case 2:
        runServer();
        break;
    case 3:
        runClient();
        break;
    default:
        std::cout << "Invalid choice. Exiting...\n";
    }

    return 0;
}
