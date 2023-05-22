//
// Created by Lara Abu Hamad on 21/05/2023.
//

#ifndef ST_REACTOR_H
#define ST_REACTOR_H

#include <map>
#include <thread>
#include <functional>
#include <poll.h>
#include <vector>

using namespace std;
typedef function<void(int)> handler_t;

class st_reactor {
private:
    map<int, handler_t> handlers;
    thread *reactor_thread;
    bool active;
public:
    st_reactor();

    ~st_reactor();

    void stopReactor();

    void startReactor();

    void addFd(int fd, handler_t handler);

    void waitFor();
};


#endif