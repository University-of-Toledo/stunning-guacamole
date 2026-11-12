CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -MMD -MP
TARGET = grade_stats

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
