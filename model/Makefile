CXX = g++
CXFLAGS = -Wall -std=c++14

default: temp.out

temp.out: temp.o Job.o Node.o LoadBalancing.o
	$(CXX) $(CXFLAGS) $? -o $@

temp.o: temp.cpp Job.h Node.h LoadBalancing.h
	$(CXX) $(CXFLAGS) -c $*.cpp

LoadBalancing.o: LoadBalancing.cpp LoadBalancing.h
	$(CXX) $(CXFLAGS) -c $*.cpp

Node.o: Node.cpp Node.h
	$(CXX) $(CXFLAGS) -c $*.cpp

Job.o: Job.cpp Job.h
	$(CXX) $(CXFLAGS) -c $*.cpp

clean:
	rm -rf *.o *.a *.out