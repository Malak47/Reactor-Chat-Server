# Reactor-Chat-Server

You need to implement a **chat that supports an unlimited number of clients using the reactor design pattern**, using the reactor structure as well as either `poll` or `select` based on your choice.

The explanation for this API, as well as code examples, can be found in the **beej guide** in chapter 7, which is highly recommended to read thoroughly and build an action plan before starting implementation.

The **reactor library** should be named `st_reactor.so`.

The **reactor should only support reading** (no writing or exceptions). According to the reactor design pattern, you should do the following:

- a. Implement a data structure that maps between the file descriptor (FD) and the function.
- b. Implement the selector mechanism responsible for listening to all FDs and executing the corresponding action specified in the reactor table. This can be done using `poll` or `select`.
- c. Since the selector listens to multiple FDs, and it's possible that more than one FD will be "ready" at a certain stage, the order of handling doesn't matter.

You can define a **struct** that holds the private members of the reactor in the desired way.


**The API** of the library should be as follows:

- a. `void* createReactor()` - Creates the reactor and returns a pointer to the reactor structure that will be passed to the next functions. When the reactor is created, it should not work, but all data structures should be initialized and allocated.
- b. `void stopReactor(void* this)` - Stops the reactor if it's active. Otherwise, it does nothing.
- c. `void startReactor(void* this)` - Starts a thread for the reactor. The thread will run in a busy loop and actually call `select` or `poll`.
- d. `void addFd(void* this, int fd, handler_t handler)` - Adds an FD to the reactor. `handler_t` is a pointer to the function that will be called when the FD is ready. You can use `typedef` for `handler_t` as you prefer.
- e. `void waitFor(void* this)` - Waits using `pthread_join` until the reactor thread finishes.

**Wrapper of the library - The application:**
The server should work exactly like the beej chat server (e.g., `selectserver`). The client that interacts with it should work in the same way as well. For any questions, such as which port to run on, etc., you should follow the same approach as in the Beej's guide.

When the reactor thread is running, the main thread can wait for it.

The execution format of the server is:

```bash
$ ./react_server
```

It doesn't require any arguments, just like in beej.
