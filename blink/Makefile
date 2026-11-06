CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lgpiod

all: blink

blink: blink.o
	$(CC) $(CFLAGS) -o blink blink.o $(LIBS)

blink.o: blink.c
	$(CC) $(CFLAGS) -c blink.c

clean:
	rm -f blink *.o
