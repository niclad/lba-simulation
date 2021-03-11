CC=g++
CFLAGS=-std=c++14 -Wall

test: test.cpp SimServer.o
	$(CC) $(CFLAGS) -o test.out $?

SimServer.o: SimServer.cpp SimServer.h
	$(CC) -c SimServer.cpp
