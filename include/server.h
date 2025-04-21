// Server.h
#ifndef SERVER_H
#define SERVER_H

#include "libraryManager.h"
#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <netinet/in.h>

class Server
{
private:
    int serverSocket;                       // Socket file descriptor
    LibraryManager libraryManager;          // To interact with the library system
    std::vector<std::thread> clientThreads; // Vector to store client threads

    void handleClient(int clientSocket, LibraryManager &libraryManager);

public:
    int port = 8080; // Port number for the server
    Server(LibraryManager *libraryManager);
    ~Server();

    void killProcessUsingPort(); // Kill the process using the port to free it
    void start();
};

#endif
