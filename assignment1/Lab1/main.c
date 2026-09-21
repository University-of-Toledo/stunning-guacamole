// Mike Fitch
// Lab 1 Hello and the toolchain
// bringing the function into the file 

#include <stdio.h>
#include "greet.h"

int main(int argc, char *argv[])
{
    const char *name = "Mike";
    if (argc > 1){
        name = argv[1];
    } else {
        printf("Hello Mike,welcome to class\n");
    }
    
    greet(name);
    
    return 0;
}
