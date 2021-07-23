# the compiler: gcc for C program, define as g++ for C++
CC = g++
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -std=c++11
TARGET = numvert
OUTPUT = 

# detect operationg system
ifeq ($(OS),Windows_NT)
		OUTPUT = -o $(TARGET).exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		OUTPUT = -o $(TARGET).out
    endif
    ifeq ($(UNAME_S),Darwin)
        OUTPUT = -o $(TARGET).out
    endif
endif

# build target executable:
all: $(TARGET)
	$(CC) $(TARGET).cpp $(OUTPUT) $(CFLAGS)

debug: $(TARGET)
	$(CC) $(TARGET).cpp $(OUTPUT) -g 

clean:
	$(RM) $(TARGET).exe
	$(RM) $(TARGET).out