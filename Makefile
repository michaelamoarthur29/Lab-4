CC = gcc
CFLAGS = -Wall -Wformat -lpthread

all: server client

server: serv.c list.c list.h
	$(CC) serv.c list.c -o server $(CFLAGS)

client: cli.c
	$(CC) cli.c -o client $(CFLAGS)

clean:
	rm -f server client
