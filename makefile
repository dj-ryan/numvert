# the compiler: gcc for C program, define as g++ for C++
CC = g++
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall

# the build target executable:
TARGET = numvert

all: $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET).out $(TARGET).cpp

debug: $(TARGET)
	$(CC) $(CFLAGS) -g -o $(TARGET).out $(TARGET).cpp

clean:
	$(RM) $(TARGET)
	$(RM) $(TARGET).out