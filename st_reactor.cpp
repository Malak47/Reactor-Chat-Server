//
// Created by Lara Abu Hamad on 21/05/2023.
//

#include "st_reactor.hpp"


st_reactor::st_reactor() : reactor_thread(nullptr), active(false) {}

st_reactor::~st_reactor() {
    stopReactor();
}

void st_reactor::stopReactor() {
    active = false;
    if (reactor_thread && reactor_thread->joinable()) {
        reactor_thread->join();
        delete reactor_thread;
        reactor_thread = nullptr;
    }
}

void st_reactor::startReactor() {
    active = true;
    reactor_thread = new thread([this]() {
        while (active) {
            vector<pollfd> fds;
            for (const auto &handler: handlers) {
                fds.push_back({handler.first, POLLIN, 0});
            }
            if (poll(fds.data(), fds.size(), 1000) > 0) { // timeout is 1000ms
                for (auto &fd: fds) {
                    if (fd.revents & POLLIN) {
                        handlers[fd.fd](fd.fd);
                    }
                }
            }
        }
    });
}

void st_reactor::addFd(int fd, handler_t handler) {
    handlers[fd] = handler;
}

void st_reactor::waitFor() {
    if (reactor_thread && reactor_thread->joinable()) {
        reactor_thread->join();
    }
}
