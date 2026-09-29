#include <stdio.h>
#include "greet.h"

int main(int argc, char *argv[])

{
	const char *name = "student";

	if (argc > 1)
	{
		name = argv[1];
	}

	else

	{
		printf("Hello my name is Corbin Glambin");
	}

	greet(name);

	return 0;
}
