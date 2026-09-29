/*
 * greet.c
 * CSET 3150 - Lab 1 demo (Hello and the Toolchain)
 * Implementation of the greeting module declared in greet.h.
 */
#include <stdio.h>
#include "hello.h"

void  greet (const char *John)
{
    printf("Hello, %s! Welcome to Class.\n", John);
}
