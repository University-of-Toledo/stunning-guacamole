# Makefile for format_demo (Lab 2 companion sample)
#
# Usage:
#   make        build the program
#   make run    build (if needed) and run it
#   make clean  delete the compiled program
#   make deploy copy the source and Makefile to the Pi (run on your Mac)

# Compiler and flags. -Wall -Wextra turn on warnings; -Werror makes
# every warning an error, as the course requires. -g adds debug info.
CC     = gcc
CFLAGS = -Wall -Wextra -Werror -g

TARGET = format_demo

# Where "make deploy" copies files. Change these if the Pi's address
# or folder changes.
PI_HOST = pi@192.168.1.100
PI_DIR  = /home/pi/stunning-guacamole/lab02
SRC    = mainconverter.c

# .PHONY marks targets that are commands, not files to build.
.PHONY: all run clean deploy

# The first target is the default one that runs when you type "make".
all: $(TARGET)

# Rebuild the program only when the .c file is newer than it.
# $@ means "the target" and $^ means "all the prerequisites".
# NOTE: the command line below MUST start with a TAB, not spaces.
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

# Copy the files to the Pi over the network. mkdir -p creates the
# folder if it doesn't exist yet. Run this from your Mac.
deploy:
	ssh $(PI_HOST) "mkdir -p $(PI_DIR)"
	scp $(SRC) Makefile $(PI_HOST):$(PI_DIR)/
