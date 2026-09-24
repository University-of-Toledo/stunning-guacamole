
#include <stdio.h>
#include "greet.h"

int main (int argc, char * argv[])
{
    const char * name = " student " ;
    
    if ( argc > 1 ) {
        name = argv[1];
    } 
    
    else {
        printf("Hello, Student what is your name? \n");
    }
 greet (name);
 
 return 0;
    
}