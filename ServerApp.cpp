//
// Created by malaklinux on 5/21/23.
//

#include <arpa/inet.h>
#include "ServerApp.hpp"

void ServerApp::startListening() {
    struct sockaddr_in serveraddr{}; // server's address information

    // get the listener
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    // bind
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT); // you can define PORT on your own
    memset(&(serveraddr.sin_zero), '\0', 8);

    if (bind(listener, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1) {
        perror("Bind");
        exit(1);
    }

    // listen
    if (listen(listener, 10) == -1) {
        perror("Listen");
        exit(1);
    }

    // add the listener to the reactor
    reactor->addFd(listener, ServerApp::handleNewClient);
}

ServerApp::ServerApp() {
    reactor = new Reactor();
}

ServerApp::~ServerApp() {
    close(listener);
    delete reactor;
}


void ServerApp::run() {
    startListening();
    reactor->startReactor();
}

void ServerApp::handleClient(int clientSock) {
    char buffer[MAX_BUFFER];

    // Clear the buffer
    memset(buffer, 0, sizeof(buffer));

    // Receive data from the client socket
    int bytesReceived = recv(clientSock, buffer, sizeof(buffer), 0);

    if (bytesReceived < 0) {
        perror("recv");
        close(clientSock);
        // Remove the client from the reactor
        instance->reactor->removeFd(clientSock);
        return;
    } else if (bytesReceived == 0) {
        // If recv returns 0, the client has closed the connection
        close(clientSock);
        instance->reactor->removeFd(clientSock);
        return;
    }

    // Print the received message
    printf("Received: %s", buffer);

    // Echo the message back to the client
    if (send(clientSock, buffer, bytesReceived, 0) != bytesReceived) {
        perror("send");
        close(clientSock);
        instance->reactor->removeFd(clientSock);
    }
}

void ServerApp::handleNewClient(int listener) {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int newfd = accept(listener, (struct sockaddr *) &clientAddr, &clientLen);

    if (newfd == -1) {
        perror("accept");
    } else {
        printf("New connection from %s on socket %d\n",
               inet_ntoa(clientAddr.sin_addr), newfd);
        instance->reactor->addFd(newfd, handleClient);
    }
}

ServerApp *ServerApp::getInstance() {
    return instance;
}



int ServerApp::main(int argc, char *argv[]) {
    // Check arguments
    if (argc != 1) {
        fprintf(stderr,"usage: ./react_server\n");
        exit(EXIT_FAILURE);
    }

    try {
        // Create a new ServerApp object
        ServerApp* serverApp = getInstance();

        // Run the server app
        serverApp->run();
        delete serverApp;

    } catch (const exception& e) {
        // Handle any exceptions that were thrown
        cerr << "Exception caught: " << e.what() << '\n';
        exit(EXIT_FAILURE);
    }

    return 0;
}
