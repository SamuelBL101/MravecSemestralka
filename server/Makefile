CC = gcc
CFLAGS = -Wall -g

.PHONY: all clean

server: server/main.c
	$(CC) $(CFLAGS) -o server main.c

all: server

clean:
	rm -f server