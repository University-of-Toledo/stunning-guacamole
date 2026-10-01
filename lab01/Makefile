CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: hello greet

hello: hello.o
	$(CC) $(CFLAGS) -c main.c
greet: main.o greet.o
	$(CC) $(Cflags) -o greet main.o greet.o
hello.o: hello.c
	$(CC) $(CFLAGS) -c main.c
greet.o: greet.c greet.h
	$(CC) $(CFLAGS) -c greet.c
clean: 
	rm -f hello greet *.o

