CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = Hello
SRCS = main.c greet.c 
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.CFLAGS  
    $(CC) $(CFLAGS) -c $< -o $@
    
clean : 
    rm -f $(OBJS) $(TARGET)
