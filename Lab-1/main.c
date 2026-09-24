
 //Name: Connor Rademaker
 //Lab 1 - Hello and ToolChain
 //calling the function in this file

#include <stdio.h>
#include "greet.h"

int main(int argc, char **argv)
{
	const char *name = "Connor";
	
	if (argc > 1) {
		name = argv[1];
	}
	else {
		printf("Hello %s\n");
	}
	
	greet(name);
	
	return 0;
}

