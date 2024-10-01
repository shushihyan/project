CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

TARGET = read-file
SOURCES = read-file.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
