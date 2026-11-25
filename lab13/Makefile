CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11 -g
LDFLAGS = -lgpiod
TARGET  = traffic_light
SRC     = traffic_light.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./$(TARGET)

clean:
	rm -f $(TARGET)
