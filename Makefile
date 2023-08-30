# Makefile for compiling the project

# Compiler
CXX := g++

# Compiler Flags
CXXFLAGS := -std=c++11

# Include paths
INCLUDES := -I/opt/homebrew/opt/cpprestsdk/include/ \
            -I/opt/homebrew/opt/boost/include \
            -I/opt/homebrew/opt/openssl@3/include

# Library paths
LIBS := -L/opt/homebrew/opt/cpprestsdk/lib \
        -L/opt/homebrew/opt/boost/lib \
        -L/opt/homebrew/opt/openssl@3/lib

# Linked libraries
LINK_FLAGS := -lcpprest -lboost_system -lssl -lcrypto

# Source Files
SRC := main.cpp

# Target executable
TARGET := main

.PHONY: all clean

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(TARGET) $(LIBS) $(LINK_FLAGS)

clean:
	rm -f $(TARGET)
