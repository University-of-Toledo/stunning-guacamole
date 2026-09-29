/*
 * main.c
 * CSET 3150 - Lab 1 demo (Hello and the Toolchain)
 * Entry point. Demonstrates the compile-link model with a program split
 * across multiple translation units (main.c + greet.c + greet.h), built
 * with a Makefile, and compiled clean under -Wall -Wextra -Werror.
 */
#include "hello.h"

int main(void)
{
    greet("John");
      return 0;
}
