CXX = g++
CC = gcc
CXFLAGS = -Wall -std=c++14
CCFLAGS = -Wall -std=c99

default: temp.out

temp.out: temp.o Job.o Node.o LoadBalancing.o rngs.o rvgs.o
	$(CXX) $(CXFLAGS) $? -o $@

temp.o: temp.cpp Job.h Node.h LoadBalancing.h
	$(CXX) $(CXFLAGS) -c $*.cpp

LoadBalancing.o: LoadBalancing.cpp LoadBalancing.h
	$(CXX) $(CXFLAGS) -c $*.cpp

Node.o: Node.cpp Node.h Job.h
	$(CXX) $(CXFLAGS) -c $*.cpp

Job.o: Job.cpp Job.h
	$(CXX) $(CXFLAGS) -c $*.cpp

rng.o: rng.c rng.h
	$(CXX) $(CXFLAGS) -c $*.c

rngs.o: rngs.c rngs.h 
	$(CC) $(CCFLAGS) -c $*.c

rvgs.o: rvgs.c rvgs.h rngs.h
	$(CC) $(CCFLAGS) -c $*.c

rvms.o: rvms.c rvms.h
	$(CXX) $(CXFLAGS) -c $*.c

clean:
	rm -rf *.o *.a *.out
