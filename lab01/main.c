#include <stdio.h>
#include "greet.h"

int main(int argc, char *argv[])
{ 
    const char *name = "student";
   
  if (argc >  1){
     name = argv[1];
  } else { 
          printf("(tip: pass your name as an argument, e.g. ./hello Merl)\n");
  }

  greet(name);

 return 0;
}
