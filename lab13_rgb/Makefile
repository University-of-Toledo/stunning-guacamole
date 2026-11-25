CC = gcc
CFLAGS = -Wall -Wextra -Werror
GPIO_FLAGS = $(shell pkg-config --cflags --libs libgpiod)

rgb: rgb.c
	$(CC) $(CFLAGS) rgb.c -o rgb $(GPIO_FLAGS)

clean:
	rm -f rgb

.PHONY: clean
