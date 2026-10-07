CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

TARGET = converter
SRC = converter.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

