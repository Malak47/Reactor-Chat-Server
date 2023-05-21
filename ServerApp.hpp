//
// Created by malaklinux on 5/21/23.
//

#ifndef REACT_CHATTOOL_SERVERAPP_HPP
#define REACT_CHATTOOL_SERVERAPP_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "Reactor.hpp"

#define PORT 1234  // or any other port number
//#define MAX_BUFFER 1024  // or any other size you prefer


class ServerApp {
private:
    Reactor *reactor;
    int listener;     // listening socket descriptor
    ServerApp *instance;  // pointer to ServerApp instance


public:
    void startListening();

    ServerApp();

    ~ServerApp();

    void run();

    void handleNewClient(int listener);

    void handleClient(int client_fd);

    ServerApp *getInstance();

    int main(int argc, char *argv[]);

};

#endif //REACT_CHATTOOL_SERVERAPP_HPP
