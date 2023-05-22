# Reactor Chat Server

This application implements a **multithreading chat server** using the **reactor design pattern**.

The **server** uses threads to
handle multiple client connections, with each **client** being handled by a separate thread.

The **server** supports an **unlimited number of clients** by dynamically storing file descriptors and handlers.

## About the Code

This project consists of two main components: the `Reactor` and the `main server application`.

1. **Reactor:** The Reactor class is a mechanism that receives a file descriptor (fd) from a client and a function to
   perform when the fd is active (has data to read).

   The Reactor supports a large number of fds, running in a single thread, and uses the poll mechanism to handle events
   on file descriptors.

2. **Server Application:** This is where the Reactor is used.
   The server application creates a socket to listen for incoming client connections. Each time a connection is
   accepted, it is added to the Reactor with a handler function.
   This handler function is responsible for receiving data from the client and broadcasting it to all other clients.

## How to Compile

You can compile the code using the provided makefile. Use the `make all` command to compile the `server application` and
the `Reactor library`, and `make clean` command to remove the compiled files.

```shell
make all
```

## How to Run

To run the `server`, use the command `./react_server`.

```shell
./react_server
```

To connect as a `client`, use `telnet <hostname> 9034` from another terminal window. The default port is 9034, and if
you are running the client on the same machine as the server, you can use `localhost` as the hostname.

```shell
telnet localhost 9034
```

## Example

1. Start the server:

    ```shell
    ./react_server
    ```

   Output:

    ```shell
    Listening on port: [9034] ...
    ```

2. Connect with a client:

    ```shell
    telnet localhost 9034
    ```

   Output:

    ```shell
    Trying ::1...
    Trying 127.0.0.1...
    Connected to localhost.
    Escape character is '^]'.
    ```

3. Send a message from the client:

    ```shell
    Hello, world!
    ```

4. The server will print the received message:

    ```shell
    [Client <ClientID>] message: Hello, world!
    ```

   All other clients will also receive this message.

---

