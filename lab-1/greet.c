//Name: Fiza Shaikh
//Lab 1 - Hello and ToolChain
//Introductory lab to C programming and getting familiar with github

#include <stdio.h>
#include "greet.h"

void greet(const char *name)
{
    if (name != NULL)
    {
        printf("Welcome to EET 3150\n", name);
    }
}
