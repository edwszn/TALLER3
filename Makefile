CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = programa
SRC = main.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
