
CC = gcc
CFLAGS = -Wall -Wextra -Werror

TARGET = grade_statistics
SRC = grade_statistics.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
