CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDLIBS = $(shell pkg-config --libs libgpiod)
CPPFLAGS = $(shell pkg-config --cflags libgpiod)

all: blink

blink: blink.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o blink blink.c $(LDLIBS)

clean:
	rm -f blink

.PHONY: all clean
