//Name: Connor Rademaker
//Lab 1 - Hello and ToolChain
//declaring the function in this file

#include <stdio.h>
#include "greet.h"

void greet(const char *name)
{
	if (name != NULL)
	{
		printf("Welcome to EET 3150\n", name);
	}
}
