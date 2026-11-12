CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g
LDFLAGS = -lgpiod

all: blink

blink: blink.c
	$(CC) $(CFLAGS) -o blink blink.c $(LDFLAGS)

clean:
	rm -f blink
