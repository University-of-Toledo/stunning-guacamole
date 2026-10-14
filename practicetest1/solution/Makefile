CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g

TARGET = test_practice
SRC = test_practice.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
