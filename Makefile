CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -Iinclude

TARGETS = server client \

all: $(TARGETS)

server: src/server.c src/termo.c
	$(CC) $(CFLAGS) -o $@ $<

client: src/client.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean
