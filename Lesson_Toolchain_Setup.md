# Tonight's Lesson: Toolchain & Environment Setup

*Companion notes for Lab 1 / Pre-Lab 1 — the build pipeline, verifying your tools, git setup, and why we compile with strict warning flags.*

## 1. The Four Stages of Building a C Program

A C program does not become an executable in one step. Each stage hands its output to the next:

1. **Preprocessing** — expands `#include` and `#define` directives and strips comments, producing pure C source with no macros or directives left in it.
2. **Compilation** — translates that preprocessed source into assembly code for the target architecture (on the course's Pi image, ARM64/AArch64).
3. **Assembly** — the assembler turns that assembly into machine code, producing an object file (`.o`) — compiled binary code, but not yet a runnable program.
4. **Linking** — the linker combines one or more object files with any needed libraries into a single executable, resolving references such as calls to `printf` across file boundaries.

`gcc` runs all four stages by default when you type `gcc file.c -o program`, but you can stop after any individual stage with `-E` (preprocess only), `-S` (through compilation), or `-c` (through assembly) — useful for seeing exactly what each stage produced when debugging a build error.

## 2. Confirm gcc, make, and git Are Installed

Open a terminal on your assigned Pi and run each of the following:

```bash
gcc --version
make --version
git --version
```

Each should print a version number. If any comes back with "command not found," the course image may not have installed cleanly, or a package is missing — the following installs all three:

```bash
sudo apt install build-essential git
```

## 3. Configure Git and Confirm Repository Access

Git needs to know who is making commits before it will let you commit at all:

```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@domain"
```

Then confirm you can actually reach your assigned repository — either clone it fresh, or, if it is already cloned, fetch without changing anything:

```bash
git clone <your-repo-url>
# or, if already cloned:
git fetch
git ls-remote <your-repo-url>
```

If your repository uses SSH rather than HTTPS, `ssh -T git@<host>` will tell you whether your SSH key is recognized. This matters because, per the syllabus, code that only exists on your Pi's local disk does not count for grading — it has to actually reach the remote.

## 4. Why -Wall -Wextra -Werror, Before You Need Them

These are three separate flags, stacking on top of each other:

- **`-Wall`** — turns on a broad set of common warnings: unused variables, suspicious comparisons, format-string mismatches, and more.
- **`-Wextra`** — turns on additional warnings beyond `-Wall`: unused function parameters, stricter sign-comparison checks, and more.
- **`-Werror`** — the important one: it promotes every warning from either flag above into a hard compile error, so the code will not build at all until every warning is resolved.

A lot of subtle C bugs — implicit type conversions, mismatched `printf` format specifiers, uninitialized variables — compile silently under default settings and only misbehave at runtime. Turning warnings into errors forces you to confront them at compile time. That is exactly why the syllabus treats a clean build under these flags as the baseline for a submission to even be considered correct, not an optional nicety.

```bash
gcc -Wall -Wextra -Werror -o program file.c
```

## Reading / Concept Check — Answers

**Q1: Name the four stages a C source file passes through on its way to an executable, and what each stage takes as input and produces as output.**

Preprocessing takes raw `.c` source (with `#include`/`#define` directives and comments) and produces pure C source with those directives expanded and comments stripped. Compilation takes that preprocessed source and produces assembly code for the target architecture. Assembly takes that assembly code and produces an object file (`.o`) — machine code, but not yet linked into a runnable program. Linking takes one or more object files plus any needed libraries and produces a single executable, resolving symbol references (such as a call to `printf`) across file boundaries.

**Q2: What does -Wall add to a gcc invocation? What does -Wextra add beyond -Wall? What does -Werror change about how warnings are treated?**

`-Wall` enables a broad set of common warning categories — unused variables, suspicious comparisons, format-string mismatches, and more. `-Wextra` enables additional warnings on top of that set — unused function parameters, stricter sign-comparison checks, and more. `-Werror` does not add new warnings at all; it changes how every warning already enabled is treated, promoting each one from a note into a hard compile error, so the file will not build until all of them are resolved.

**Q3: What is the difference between compiling a file with gcc -c versus without -c?**

`gcc -c` runs preprocessing, compilation, and assembly, then stops — it produces a relocatable object file (`.o`) and does not link. This is what you use to compile each source file in a multi-file program separately, before linking them all together in a later step. `gcc` without `-c` runs the full pipeline through linking as well, producing a complete executable directly (named `a.out` by default, or whatever you pass to `-o`).

**Q4: In a Makefile rule, what are the target, the prerequisites, and the recipe?**

A Makefile rule has the shape `target: prerequisites`, followed by an indented recipe. The target is the file the rule produces — the thing being built, written to the left of the colon. The prerequisites are the files that target depends on, written to the right of the colon; if any prerequisite is newer than the target, the target needs rebuilding. The recipe is the shell command (or commands) on the indented line(s) below — indented with an actual tab character, not spaces — that `make` actually runs to build the target from its prerequisites.

**Q5: Why does make usually avoid rebuilding a target that is already up to date, and how does it decide?**

Rebuilding everything on every invocation would defeat the point of separate compilation on any project with more than one source file. `make` compares file modification timestamps: if a target already exists and is newer than every one of its prerequisites, `make` considers it up to date and skips the recipe entirely. It decides this by walking the dependency graph recursively — a target is rebuilt if it does not exist yet, if any direct prerequisite is newer than it, or if any prerequisite is itself out of date and therefore gets rebuilt first, which can cascade up the chain.

**Q6: What is the difference between a local git commit and pushing that commit — why can a commit exist and still not count as submitted work for this course?**

A commit only changes the history of your local git repository — the copy of the repo sitting on your Pi's disk. It exists nowhere else until you push it. Pushing sends those committed changes to the remote repository (your assigned GitHub/GitLab repo), which is the only copy the instructor or grader can actually see. Per the syllabus, work that only exists on your Pi's local disk does not count as submitted for this course — storage-card failure is explicitly not an accepted reason for a missing submission — so a commit protects your local history, but only a push actually delivers the work for grading.

## Makefile Walkthrough

Here is a small, complete Makefile for a two-file program (`main.c` and `greet.c`, building an executable called `hello`), annotated line by line below it.

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = hello
SRCS = main.c greet.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
```

**Variables (`CC`, `CFLAGS`, `TARGET`, `SRCS`, `OBJS`)** — defined once at the top so the compiler name, flags, and file lists are not repeated (and can be changed in one place). `OBJS = $(SRCS:.c=.o)` is a substitution: it takes the `SRCS` list and replaces every `.c` extension with `.o`, so `OBJS` automatically becomes "main.o greet.o" without retyping it.

**`.PHONY: all clean`** — tells `make` that `all` and `clean` are not real files it should check timestamps for. Without this, if a file literally named `clean` ever existed in the directory, `make` would think the `clean` target was already up to date and refuse to run it.

**`all: $(TARGET)`** — the first rule in the file, so it is the default goal — what runs when you type plain `make` with no arguments. It has no recipe of its own; it just depends on `$(TARGET)`, which forces that rule to run.

**`$(TARGET): $(OBJS)`** — the target is the `hello` executable; the prerequisites are both object files. If either `main.o` or `greet.o` is newer than `hello` (or `hello` does not exist yet), the recipe below runs and links them into the executable.

**`%.o: %.c`** — a pattern rule: an implicit recipe for turning any `X.c` into `X.o`, so you do not need a separate rule for `main.o` and another for `greet.o`. `$<` is an automatic variable meaning "the first prerequisite" (the `.c` file); `$@` means "the target" (the `.o` file being built).

**`clean:`** — a target with no prerequisites at all, so it never runs automatically — only when you explicitly type `make clean`. Its recipe deletes every build artifact, giving you a clean slate.

**Try it:** run `make` (builds `hello`), `touch main.c` (updates its timestamp), then run `make` again — only `main.o` and `hello` rebuild, not `greet.o`, because `greet.c` did not change. That is `make`'s dependency tracking in action.

## Tonight's Checklist

- [ ] Run `gcc --version`, `make --version`, and `git --version` — all three return a version number.
- [ ] `git config --global user.name` / `user.email` set to your own identity.
- [ ] Successfully clone or fetch your assigned repository.
- [ ] Compile a trivial C file with `-Wall -Wextra -Werror` and confirm it builds clean.
- [ ] Write and run the sample Makefile above; confirm `make clean` followed by `make` rebuilds everything, and a second plain `make` rebuilds nothing.
