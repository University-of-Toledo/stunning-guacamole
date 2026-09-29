/*
 * greet.c
 * CSET 3150 - Lab 1 demo (Hello and the Toolchain)
 * Implementation of the greeting module declared in greet.h.
 */
#include <stdio.h>
#include "greet.h"

void greet(const char *name)
{
    printf("Hello, %s! Welcome to class.\n", name);
}
