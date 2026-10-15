CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -MMD -MP
TARGET = hello


SRCS = \*.c
OBJS = \*.o
DEPS = \*.d

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)


$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
