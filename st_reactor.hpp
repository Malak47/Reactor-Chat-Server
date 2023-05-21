//
// Created by Lara Abu Hamad on 21/05/2023.
//

#ifndef ST_REACTOR_H
#define ST_REACTOR_H

#include <map>
#include <thread>
#include <functional>

typedef std::function<void(int)> handler_t;

class Reactor {
private:
    std::map<int, handler_t> handlers;
    std::thread* reactor_thread;
    bool active;
public:
    Reactor();
    ~Reactor();

    void stopReactor();
    void startReactor();
    void addFd(int fd, handler_t handler);
    void waitFor();
};


#endif