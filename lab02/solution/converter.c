/*
Author: Jonathan Rodriguez
Exercise: Lab02: Unit Converter
Description: C program that converts Celsius to Fahrenheit, miles to kilometers,and pounds to kilograms. 
*/
#include <stdio.h>

double celsius_to_fahrenheit(double celsius){
	return celsius * (9.0 / 5.0) + 32.0;
	}

double miles_to_kilometers(double miles){
	return miles * 1.6093;
	}

double pounds_to_kilograms(double pounds){
	return pounds * 0.4535;
	}

int main(void)
	{

	int choice;

	double celsius;
	double fahrenheit;
	double miles;
	double kilometers;
	double pounds;
	double kilograms;
	/* 
	* 7/2 gives 3 because both numbers are integers.
	*Integer division drops the decimal part.
	* 7.0/2 gives 3.5 becasue 7.0 is a double.
	* (double)7/2 also gives 3.5 because we convert 7 to a double.
	*/
	printf("\n-- division --\n");
	printf("7/2 = %d\n", 7/2);
	printf("7.0/2 = %.1f\n", 7.0/2);
	printf("(double)7/2 = %.1f\n\n", (double)7/2);

	do{
		printf("Unit Converter\n");
		printf("1. Celsius to Fahrenheit\n");
		printf("2. Miles to Kilometers\n");
		printf("3.Pounds to Kilograms\n");
		printf("4.Exit\n");
		printf("Enter your choice\n");

		if (scanf("%d", &choice) != 1){
			printf("Invalid input. Please enter a number.\n");
			return 1;
			}

		switch (choice){
			case 1: 
			printf("You selected Celsius to Fahrenheit\n");
			printf("Enter temperature in celsius\n\n");

			if (scanf("%lf", &celsius) !=1){
				printf("Invalid Input.\n");
				return 1;
				}
			fahrenheit = celsius_to_fahrenheit(celsius);
			printf("%.2f Celsius = %.2f Fahrenheit\n\n", celsius, fahrenheit);
			break;

			case 2:
			printf("You selected Miles to Kilometers\n");
			printf("Enter the distance in miles\n\n");

			if (scanf("%lf", &miles) !=1){
				printf("Invalid Input.\n");
				return 1;
				}
			kilometers = miles_to_kilometers(miles);
			printf("%.2f miles = %.2f kilometers\n\n", miles, kilometers);
			break;

			case 3:
			printf("You selected Pounds to Kilograms\n\n");
			printf("Enter weight in pounds\n");

			if (scanf("%lf", &pounds) !=1){
				printf("Invalid Input.\n");
				return 1;
				}
			kilograms = pounds_to_kilograms(pounds);
			printf("%.2f pounds = %.2f kilograms\n\n", pounds, kilograms);
			break;

			case 4:
			printf("You selected Exit\n\n");
			break;

			default :
			printf("Invalid option\n");
			break;
			}

	}while (choice !=4);

return 0;

}
