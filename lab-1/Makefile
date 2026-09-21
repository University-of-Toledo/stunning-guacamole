//Name: Fiza Shaikh
//Lab 1 - Hello and ToolChain
//Code to link multi file system

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = hello
SRCS = main.c greet.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
