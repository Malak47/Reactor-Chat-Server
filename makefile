CC = g++
CFLAGS = -std=c++11 -fPIC
LIBS = -lpthread

all: react_server st_reactor.so

react_server: react_server.cpp Reactor.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

st_reactor.so: Reactor.o
	$(CC) -shared -o $@ $<

Reactor.o: Reactor.cpp Reactor.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o react_server st_reactor.so
