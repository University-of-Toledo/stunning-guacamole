CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: hello greet

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c
greet: main.o greet.o
	$(CC) $(CFLAGS) -o greet main.o greet.o
main.o: main.c greet.h
	$(CC) $(CFLAGS) -c main.c
greet.o: greet.c greet.h
	$(CC) $(CFLAGS) -c greet.c
clean:
	rm -f hello greet main.o greet.o
