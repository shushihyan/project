CXX = g++
CXXFLAGS = -Wall -Wextra

all: simple-copy

simple-copy: simple-copy.o
	$(CXX) $(CXXFLAGS) simple-copy.o -o simple-copy

simple-copy.o: simple-copy.cpp
	$(CXX) $(CXXFLAGS) -c simple-copy.cpp -o simple-copy.o

clean:
	rm -f *.o simple-copy