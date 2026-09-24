/*
 * main.c
 * CSET 3150 - Lab 1 (Hello and the Toolchain) */
#include <stdio.h>
#include <stdlib.h>
#include "greet.h"

int main(int argc, char *argv[])
{
    const char *name = "student";

    if (argc > 1) {
        name = argv[1];
    } else {
        printf("(tip: pass your name as an argument, e.g. ./hello Steven)\n");
    }

    greet(name);

    return EXIT_SUCCESS;
}
