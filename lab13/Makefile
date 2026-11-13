
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDLIBS = $(shell pkg-config --libs libgpiod)

TARGET = traffic_light
SRC = traffic_light.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
