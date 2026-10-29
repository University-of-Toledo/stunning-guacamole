#include <stdio.h>

int isEven(int n);
double average(int total, int count);
void printRow(int value);

int main(void) {
	int n;
	int sum = 0;
	int even_count = 0;

	printf("Enter a number: ");
	if (scanf("%d", &n) != 1 || n < 1) {
		printf("Invalid\n");
		return 0;
	}
	for (int i = 1; i <= n; i++) {
		printRow(i);
		sum += i;
		if (isEven(i)) {
			even_count++;
		}
	}
	printf("Sum: %d\n", sum);
	printf("Even count: %d\n", even_count);
	printf("Average: %.2f\n", average(sum, n));
	return 0;
}
int isEven(int n) {
	return (n % 2 == 0);
}
double average(int total, int count) {
	return (double)total / count;
}
void printRow(int value) {
	if (isEven(value)) {
		printf("%4d even\n", value);
	} else {
		printf("%4d odd\n", value);
	}
}

