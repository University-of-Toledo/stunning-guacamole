/* 
Author: Jonathan Rodriguez
Exercise: Practice Test - Question 25
Description: Prints even or odd numbers, their sum, and average.
*/
#include <stdio.h>

int main(void)
	{
	int n;
	int sum = 0;

	printf("Enter a number: ");
	scanf("%d", &n);
	if (n<1){
		printf("Invalid\n");
		return 0;
		}
	for (int number = 1; number <= n; number++){
		if (number % 2 == 0){
			printf("%d even\n", number);
			} else {
				printf("%d odd\n", number);
				}
		sum = sum + number;
		}
	printf("Sum: %d\n", sum);
	double average = (double)sum / n;
	printf("Average: %.2f\n", average);
	}
