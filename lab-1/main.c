//Name: Fiza Shaikh
//Lab 1 - Hello and ToolChain
//Introductory lab to C programming and getting familiar with github

#include <stdio.h>
#include "greet.h"

int main(int argc, char *argv[])
{
    const char *name = "Fiza";

    if (argc > 1) {
        name = argv[1];
    } else {
        printf("Hello %\s!\n", name);
    }

    greet(name);

    return 0;
}
