# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -std=gnu99

# Source files
SRCS = main.c
OBJS = main.o

# Header file
HDRS = main.h

# Executable name
TARGET = movies

# Default target
all: $(TARGET)

# Compile source files into object files
main.o: main.c $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

# Linking the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up the object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
