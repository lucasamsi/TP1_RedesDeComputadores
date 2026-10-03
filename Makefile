CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

all: server client

server: src/server.c src/termo.c
	$(CC) $(CFLAGS) -o $@ $^

client: src/client.c src/termo.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f server client *.o

.PHONY: all clean
