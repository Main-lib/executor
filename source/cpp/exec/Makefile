CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I..

# Define the target
all: test_naming_conventions

# Compile the test program
test_naming_conventions: test_naming_conventions.cpp naming_conventions.hpp
	$(CXX) $(CXXFLAGS) -o test_naming_conventions test_naming_conventions.cpp

# Run the test
test: test_naming_conventions
	./test_naming_conventions

# Clean up
clean:
	rm -f test_naming_conventions

.PHONY: all test clean
