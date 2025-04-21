// Client.h
#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

class Client
{
private:
    int clientSocket; // Socket file descriptor

public:
    Client();
    ~Client();
    void start();
};

#endif
