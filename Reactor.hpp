//
// Created by malaklinux on 5/21/23.
//

#ifndef REACT_CHATTOOL_REACTOR_HPP
#define REACT_CHATTOOL_REACTOR_HPP

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <sys/select.h>
#include <unistd.h>
// Maximum number of file descriptors that select can handle
#define MAX_BUFFER 1024  // or any other size you prefer


using namespace std;

// Typedef for the handler function
typedef function<void(int)> handler_t;


class Reactor {
private:
    thread *reactor_thread;
    map<int, handler_t> fd_map;
    fd_set master_fds; // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number
    bool active;
    mutex mtx;   // Mutex for thread safety

    void run();

    void handle_fds();

public:
    Reactor();

    ~Reactor();

    void startReactor();

    void stopReactor();

    void addFd(int fd, handler_t handler);

    void removeFd(int fdToRemove);

    void waitFor();
};


#endif //REACT_CHATTOOL_REACTOR_HPP
