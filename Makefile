CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

LIBS = -lws2_32 -lmariadb

SRC = \
	src/*.c \
	src/libs/*.c \
	src/store/*.c \
	src/config/*.c \
	src/db/*.c 

TARGET = build/server.exe

.PHONY: build run clean

build:
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: build
	./$(TARGET)

clean:
	rm -f build/*.exe