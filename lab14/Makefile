
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDLIBS = $(shell pkg-config --libs libgpiod)
TARGET = button_debounce

all: $(TARGET)

$(TARGET): button_debounce.c
	$(CC) $(CFLAGS) button_debounce.c -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
