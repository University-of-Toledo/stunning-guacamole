CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -MMD -MP
TARGET = hello

# Automatically collect all .c files and determine output object/dependency names
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

.PHONY: all clean run

all: $(TARGET)

# Link object files to create the final executable binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile individual source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

# Shortcut target to build and immediately execute the code
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
