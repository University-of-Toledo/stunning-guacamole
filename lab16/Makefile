
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDLIBS = $(shell pkg-config --libs libgpiod)
CPPFLAGS = $(shell pkg-config --cflags libgpiod)

TARGET = morse

all: $(TARGET)

$(TARGET): morse.c
	$(CC) $(CPPFLAGS) $(CFLAGS) morse.c -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
