//
// Created by Lara Abu Hamad on 21/05/2023.
//

#include "st_reactor.hpp"
#include <poll.h>
#include <vector>

Reactor::Reactor() : reactor_thread(nullptr), active(false) {}

Reactor::~Reactor() {
    stopReactor();
}

void Reactor::stopReactor() {
    active = false;
    if (reactor_thread && reactor_thread->joinable()) {
        reactor_thread->join();
        delete reactor_thread;
        reactor_thread = nullptr;
    }
}

void Reactor::startReactor() {
    active = true;
    reactor_thread = new std::thread([this]() {
        while (active) {
            std::vector<pollfd> fds;
            for (const auto &handler : handlers) {
                fds.push_back({handler.first, POLLIN, 0});
            }
            if (poll(fds.data(), fds.size(), 1000) > 0) { // timeout is 1000ms
                for (auto &fd : fds) {
                    if (fd.revents & POLLIN) {
                        handlers[fd.fd](fd.fd);
                    }
                }
            }
        }
    });
}

void Reactor::addFd(int fd, handler_t handler) {
    handlers[fd] = handler;
}

void Reactor::waitFor() {
    if (reactor_thread && reactor_thread->joinable()) {
        reactor_thread->join();
    }
}
