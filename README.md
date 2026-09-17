# CSET 3150 — Pre-Lab 1 / Lab 1 Demo: Hello and the Toolchain

This folder contains a small two-file C program (`main.c` + `greet.c` / `greet.h`)
used to demonstrate the compile-link model and basic `make` mechanics.

## Files

- `main.c` — entry point, reads an optional name from the command line
- `greet.c` / `greet.h` — a small greeting module (separate translation unit)
- `Makefile` — automates the build

## Option 1: Compile by hand with gcc

This is the "what's actually happening" version — one command, both source
files, straight to a finished executable:

```
gcc -Wall -Wextra -Werror -std=c11 -o hello main.c greet.c
```

Run it:

```
./hello
./hello Merl
```

Clean up manually (no Makefile involved, so no `make clean`):

```
rm -f hello
```

### The same thing, broken into stages

If you want to see each file compiled separately before linking:

```
gcc -Wall -Wextra -Werror -std=c11 -c main.c -o main.o
gcc -Wall -Wextra -Werror -std=c11 -c greet.c -o greet.o
gcc -o hello main.o greet.o
```

The first two commands turn each `.c` file into an object file (`.o`).
The third links those object files into the final `hello` executable.

## Option 2: Build with make

This is what you'll actually use for every lab going forward. It runs the
same `gcc` commands above, automatically, and only rebuilds files that
changed:

```
make
```

Run it the same way:

```
./hello
./hello Merl
```

Try it again — nothing to do, since nothing changed:

```
make
```

Change one file and rebuild — only that file (and the final link step)
should recompile:

```
touch main.c
make
```

Clean up build artifacts:

```
make clean
```

## Standing requirement

Every build in this class must compile with **zero warnings** under:

```
-Wall -Wextra -Werror
```

If you see any warning, fix it before moving on — `-Werror` turns warnings
into hard errors, so the build won't succeed until it's clean.
# stunning-guacamole
