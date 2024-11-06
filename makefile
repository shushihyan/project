CXX = g++
TARGET = redirect-cin

all: redirect-cin

redirect-cin: redirect-cin.cpp
 $(CXX) -o redirect-cin redirect-cin.cpp

clean:
 rm -f redirect-cin