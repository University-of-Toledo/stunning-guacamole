#include <stdio.h>

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	
	int n;
	int sum = 0;
	
	printf("Enter a number: ");
	scanf("%d", &n);
	
	if (n < 1) {
		printf("Invalid\n");
		return 0;
	}
	
	for (int i = 1; i <= n; i++) {
		if (i % 2 == 0)
			printf("%d even\n", i);
		else
			printf("%d odd\n", i);
		sum += i;
	}
	
	double average = (double)sum / n;
	printf("Sum: %d\n", sum);
	printf("Average: %.2f\n", average);
	return 0;
}