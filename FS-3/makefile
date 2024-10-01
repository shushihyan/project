CXX = g++
CXXFLAGS = -Wall -Wextra

all: hacker-rm

hacker-rm: hacker-rm.o
	$(CXX) $(CXXFLAGS) hacker-rm.o -o hacker-rm

hacker-rm.o: hacker-rm.cpp
	$(CXX) $(CXXFLAGS) -c hacker-rm.cpp -o hacker-rm.o

clean:
	rm -f *.o hacker-rm