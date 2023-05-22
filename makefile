CC = g++
CFLAGS = -std=c++11 -fPIC
LIBS = -lpthread

all: react_server st_reactor.so

react_server: react_server.cpp st_reactor.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

st_reactor.so: st_reactor.o
	$(CC) -shared -o $@ $<

st_reactor.o: st_reactor.cpp st_reactor.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o react_server st_reactor.so
