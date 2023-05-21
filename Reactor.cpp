//
// Created by malaklinux on 5/21/23.
//

#include "Reactor.hpp"

void Reactor::run() {
    while (active) {
        handle_fds();
    }
}

void Reactor::handle_fds() {
    read_fds = master_fds; // copy it
    if (select(fdmax + 1, &read_fds, nullptr, nullptr, nullptr) == -1) {
        perror("select");
        exit(4);
    }

    // run through the existing connections looking for data to read
    for (int i = 0; i <= fdmax; i++) {
        if (FD_ISSET(i, &read_fds)) { // we got one!!
            mtx.lock();
            if (fd_map.find(i) != fd_map.end()) {
                fd_map[i](i); // Call the handler
            }
            mtx.unlock();
        }
    }
}

Reactor::Reactor() : active(false), fdmax(0) {
    FD_ZERO(&master_fds); // clear the master and temp sets
    FD_ZERO(&read_fds);
    reactor_thread = new std::thread(&Reactor::run, this);
}

Reactor::~Reactor() {
    stopReactor();
    delete reactor_thread;
}

void Reactor::startReactor() {
    mtx.lock();
    active = true;
    mtx.unlock();
}

void Reactor::stopReactor() {
    mtx.lock();
    if (active) {
        active = false;
    }
    mtx.unlock();
}

void Reactor::addFd(int fd, handler_t handler) {
    mtx.lock();
    FD_SET(fd, &master_fds); // add to master set
    if (fd > fdmax) {    // keep track of the maximum
        fdmax = fd;
    }
    fd_map[fd] = handler;
    mtx.unlock();
}

void Reactor::removeFd(int fdToRemove) {
    lock_guard<mutex> lock(mtx);

    // remove fd from map
    fd_map.erase(fdToRemove);

    // remove fd from master set
    FD_CLR(fdToRemove, &master_fds);

    // update fdmax if necessary
    if (fdToRemove == fdmax) {
        fdmax--;
    }

    close(fdToRemove);
}


void Reactor::waitFor() {
    if (reactor_thread->joinable()) {
        reactor_thread->join();
    }
}

