////
//// Created by Lara Abu Hamad on 21/05/2023.
////
//
//#include <iostream>
//#include <cstring>
//#include <unistd.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <sys/poll.h>
//#include <vector>
//#include <thread>
//
//constexpr int BUFFER_SIZE = 1024;
//
//typedef void (*handler_t)(int);
//
//struct Reactor {
//    std::vector<struct pollfd> fds;
//    std::vector<handler_t> handlers;
//
//    void createReactor() {}
//
//    void stopReactor() {}
//
//    void addFd(int fd, handler_t handler) {
//        struct pollfd pfd;
//        pfd.fd = fd;
//        pfd.events = POLLIN;
//        pfd.revents = 0;
//        fds.push_back(pfd);
//        handlers.push_back(handler);
//    }
//
//    void startReactor() {
//        while (true) {
//            int activity = poll(&fds[0], fds.size(), -1);
//            if (activity == -1) {
//                std::cerr << "Error in poll" << std::endl;
//                break;
//            }
//
//            for (size_t i = 0; i < fds.size(); ++i) {
//                if (fds[i].revents & POLLIN) {
//                    handlers[i](fds[i].fd);
//                }
//            }
//        }
//    }
//
//    void waitForReactor() {}
//};
//
//void handleClientConnection(int clientSocket) {
//    char buffer[BUFFER_SIZE];
//    ssize_t bytesRead;
//
//    while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
//        buffer[bytesRead] = '\0';
//        std::cout << "Received message: " << buffer << std::endl;
//
//        // Echo the message back to the client
//        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
//            std::cerr << "Error in send" << std::endl;
//            break;
//        }
//    }
//
//    if (bytesRead == -1) {
//        std::cerr << "Error in recv" << std::endl;
//    }
//
//    // Close the client socket
//    close(clientSocket);
//}
//
//int main() {
//    Reactor reactor;
//    int serverSocket, clientSocket;
//    struct sockaddr_in serverAddr, clientAddr;
//    socklen_t clientLen = sizeof(clientAddr);
//
//    // Create the server socket
//    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//    if (serverSocket == -1) {
//        std::cerr << "Error creating server socket" << std::endl;
//        return 1;
//    }
//
//    // Set socket options to allow address reuse
//    int enable = 1;
//    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
//        std::cerr << "Error setting socket options" << std::endl;
//        close(serverSocket);
//        return 1;
//    }
//
//    // Bind the server socket to a port
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_addr.s_addr = INADDR_ANY;
//    serverAddr.sin_port = htons(12345);
//    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
//        std::cerr << "Error binding server socket" << std::endl;
//        close(serverSocket);
//        return 1;
//    }
//
//    // Listen for incoming connections
//    if (listen(serverSocket, SOMAXCONN) == -1) {
//        std::cerr << "Error listening for connections" << std::endl;
//        close(serverSocket);
//        return 1;
//    }
//
//    // Create the reactor
//    reactor.createReactor();
//
//    // Add the server socket to the reactor
//    reactor.addFd(serverSocket, [](int fd) {
//        int clientSocket;
//        struct sockaddr_in clientAddr;
//        socklen_t clientLen = sizeof(clientAddr);
//        clientSocket = accept(fd, (struct sockaddr *) &clientAddr, &clientLen);
//        if (clientSocket == -1) {
//            std::cerr << "Error accepting client connection" << std::endl;
//            return;
//        }
//
//        std::cout << "Accepted client connection" << std::endl;
//
//        // Handle the client connection in a separate thread
//        std::thread clientThread(handleClientConnection, clientSocket);
//        clientThread.detach();
//    });
//
//
//    // Start the reactor
//    reactor.startReactor();
//
//    // Close the server socket
//    close(serverSocket);
//
//    return 0;
//}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "st_reactor.hpp"
#include <unistd.h>  // for close()

int main() {
    // create a socket
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3490);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listener, (struct sockaddr *)&addr, sizeof(addr));
    listen(listener, 10);

    // create a reactor
    Reactor reactor;

    // define a handler
    handler_t handler = [](int fd) {
        char buf[1024];
        int bytesReceived = recv(fd, buf, sizeof(buf) - 1, 0);
        if (bytesReceived > 0) {
            buf[bytesReceived] = '\0';  // Ensure null-terminated
            std::cout << "Received data: " << buf << std::endl;
        } else if (bytesReceived == 0) {
            std::cout << "Client disconnected." << std::endl;
            close(fd);  // Close the client socket
        } else {
            std::cerr << "Error occurred during recv." << std::endl;
        }
    };

    // define a listener handler to accept new connections
    handler_t listenerHandler = [&reactor, &handler](int listener) {
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int clientFd = accept(listener, (struct sockaddr *)&clientAddr, &addrLen);
        if (clientFd >= 0) {
            reactor.addFd(clientFd, handler);  // Add the new client to the reactor
        }
    };

    // add the listener to the reactor with the listener handler
    reactor.addFd(listener, listenerHandler);

    // start the reactor
    reactor.startReactor();

    // wait for the reactor
    reactor.waitFor();

    return 0;
}
