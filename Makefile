CC = gcc
CFLAGS = -g -I C:/SFML-2.6.1/include
LDFLAGS = -L C:/SFML-2.6.1/lib -lsfml-system -lsfml-window -lsfml-graphics -lsfml-network -lsfml-audio -lpthread -lws2_32

.PHONY: all server client clean

server: server/main.c server/pos_sockets/active_socket.c server/pos_sockets/char_buffer.c server/pos_sockets/passive_socket.c
	$(CC) $(CFLAGS) -o server server/main.c server/pos_sockets/active_socket.c server/pos_sockets/char_buffer.c server/pos_sockets/passive_socket.c

client: main.cpp my_socket.cpp Block.cpp World.cpp Ant.cpp Button.cpp
	$(CC) $(CFLAGS) -o client main.cpp my_socket.cpp Block.cpp World.cpp Ant.cpp Button.cpp

all: server client

clean:
	rm -f server