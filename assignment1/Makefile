CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

.PHONY: all clean

all: hello multifile

hello: hello.c
	$(CC) $(CFLAGS) hello.c -o hello

multifile: main.o greet.o
	$(CC) $(CFLAGS) -o multifile main.o greet.o 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o hello multfile
