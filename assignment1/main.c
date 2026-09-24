/*
 * main.c
 * CSET 3150 - Lab 1 demo (Hello and the Toolchain)
 * Entry point. Demonstrates the compile-link model with a program split
 * across multiple translation units (main.c + greet.c + greet.h), built
 * with a Makefile, and compiled clean under -Wall -Wextra -Werror.
 */
#include <stdio.h>
#include "greet.h"

int main(int argc, char *argv[])
{
    const char *name = "student";
    if (argc > 1) {
        name = argv[1];
    } else {
        printf("(tip: pass your name as an argument, e.g. ./hello Merl)\n");
    }

    greet(name);

    printf("Edited\n");

    int numer = 10;
    numer++;

    printf("%d", numer);
    return 0;
}
