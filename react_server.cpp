//
// Created by Lara Abu Hamad on 21/05/2023.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "Reactor.hpp"
#include <unistd.h>  // for close()
#include <set>

using namespace std;

int main() {
    // create a socket
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9034);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listener, (struct sockaddr *) &addr, sizeof(addr));
    listen(listener, 10);

    // create a reactor
    Reactor reactor;
    cout << "Listening on port: [9034] ..." << endl;

    // Define a container to hold all clients.
    set<int> clients;

    // define a handler
    handler_t handler = [&clients](int fd) {
        char buf[1024];
        int bytesReceived = recv(fd, buf, sizeof(buf) - 1, 0);
        if (bytesReceived > 0) {
            buf[bytesReceived] = '\0';  // Ensure null-terminated
            cout << "[Client " << fd << "] message: " << buf;

            // Prepare the message to be forwarded
            string forwardMsg = "[Client " + to_string(fd) + "] message: " + string(buf);

            // Forward the message to all clients
            for (int clientFd: clients) {
                if (clientFd != fd) {  // Don't send the message to the sender
                    send(clientFd, forwardMsg.c_str(), forwardMsg.length(), 0);
                }
            }
        } else if (bytesReceived == 0) {
            cout << "Client " << fd << " disconnected." << endl;
            clients.erase(fd);  // Remove the client from our list
            close(fd);  // Close the client socket
        } else {
            cerr << "Error occurred during recv from client " << fd << endl;
        }
    };

    // define a listener handler to accept new connections
    handler_t listenerHandler = [&reactor, &handler, &clients](int listener) {
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int clientFd = accept(listener, (struct sockaddr *) &clientAddr, &addrLen);
        if (clientFd >= 0) {
            clients.insert(clientFd);  // Add the new client to our list
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
