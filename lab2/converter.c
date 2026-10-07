/*
 *Steven Cooper
 * Lab 2: Unit Converter
 * Menu driven unit converter with inputs
*/

#include <stdio.h>

double f2m(double f) {return f * 0.3048;}
double i2c(double i) {return i * 2.54;}
double f2c(double f) {return (f - 32.0) * (5.0 / 9.0);}

int main(void) {
	int choice = 0;
	double val = 0.0;

	while (choice != 4) {
		printf("\n1. Feet->Meters\n2. Inches->CM\n3. F->C\n4. Exit\nEnter choice: ");
		if (scanf("%d", &choice) != 1) {
			while (getchar() != '\n'); // Clear bad input
			continue;
	}

		if (choice >=1 && choice <=3) {
			printf("Enter value to convert: ");
			if (scanf("%lf", &val) == 1) {
			if (choice == 1) printf("%.2f ft = %.2f m\n", val, f2m(val));
			if (choice == 2) printf("%.2f in = %.2f cm\n", val, i2c(val));
			if (choice == 3) printf("%.2f F = %.2f C\n", val, f2c(val));
	} else {
		printf("Invalid value entered.\n");
		while (getchar() !='\n'); // Clear bad input
	}
	}
}

printf("\nInt div:7/2 = %d | Float div: 7.0/2 = %.1f\n", 7/2, 7.0/2);
return 0;
}
