/*
 * Author:      Fiza Shaikh
 * Exercise:    Lab 2 - Unit Converter
 * Description: A menu-driven program that does different unit conversions.
 *              It includes input validation to stop bad inputs, and shows 
 *              the difference between integer and double division.
 *
 * Build:  gcc -Wall -Wextra -Werror -o converter converter.c
 * Run:    ./converter
 */

#include <stdio.h>

/* Clears out bad characters left in the input buffer if scanf fails */
static void clear_line(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard */
    }
}

/* Converts Celsius to Fahrenheit */
double celsius_to_fahrenheit(double c)
{
    return (c * 9.0 / 5.0) + 32.0;
}

/* Converts Miles to Kilometers */
double miles_to_kilometers(double miles)
{
    return miles * 1.609344;
}

/* Converts Pounds to Kilograms */
double pounds_to_kilograms(double lbs)
{
    return lbs * 0.45359237;
}

int main(void)
{
    int choice = 0;
    double input_val = 0.0;

    /*    Integer vs. floating
     *    7 / 2 drops the fraction because both numbers are integers.
     *    7.0 / 2 retains the fraction because 7.0 is a double. */
    printf("Division\n");
    printf("7 / 2         = %d\n", 7 / 2);
    printf("7.0 / 2       = %.1f\n", 7.0 / 2);
    printf("\n\n");
    

    /* Conversion Menu */
    while (1) {
        printf("--- Unit Converter Menu ---\n");
        printf("1. Celsius to Fahrenheit\n");
        printf("2. Miles to Kilometers\n");
        printf("3. Pounds to Kilograms\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");

        /* Loop until we get a valid input */
        while (scanf("%d", &choice) != 1) {
            if (feof(stdin)) {
                printf("\nNo input. Exiting.\n");
                return 1;
            }
            printf("  Not a valid option. Try again: ");
            clear_line();
        }

        /* exit option to stop program */
        if (choice == 4) {
            printf("Goodbye!\n");
            break;
        }

        /* verify the range of input */
        if (choice < 1 || choice > 4) {
            printf("  Number out of range. Choose 1 to 4.\n\n");
            continue;
        }

        /* double value to convert */
        printf("Enter the value to convert: ");
        while (scanf("%lf", &input_val) != 1) {
            if (feof(stdin)) {
                printf("\nNo input. Exiting.\n");
                return 1;
            }
            printf("  Not a valid number. Try again: ");
            clear_line();
        }

        /* Using switch function to choose menu option */
        switch (choice) {
            case 1:
                printf("  %.2f Celsius = %.2f Fahrenheit\n\n", 
                       input_val, celsius_to_fahrenheit(input_val));
                break;
            case 2:
                printf("  %.2f Miles = %.2f Kilometers\n\n", 
                       input_val, miles_to_kilometers(input_val));
                break;
            case 3:
                printf("  %.2f Pounds = %.2f Kilograms\n\n", 
                       input_val, pounds_to_kilograms(input_val));
                break;
            default:
                break;
        }
    }

    return 0;
}
/*
 * 
 * Path 1: Celsius to Fahrenheit
 * Menu Choice (Valid):       1
 * Enter value (Valid):       100
 * Output:                    100.00 Celsius = 212.00 Fahrenheit
 *
 * Menu Choice (Valid):       1
 * Enter value (Invalid):     abc
 * Output:                    Not a valid number. Try again: 0
 * Output:                    0.00 Celsius = 32.00 Fahrenheit
 * 
 * Path 2: Miles to Kilometers 
 * Menu Choice (Valid):       2
 * Enter value (Valid):       10
 * Output:                    10.00 Miles = 16.09 Kilometers
 * 
 * Menu Choice (Valid):       2
 * Enter value (Invalid):     xyz
 * Output:                    Not a valid number. Try again: 5.5
 * Output:                    5.50 Miles = 8.85 Kilometers
 * 
 * Path 3: Pounds to Kilograms 
 * Menu Choice (Valid):       3
 * Enter value (Valid):       150
 * Output:                    150.00 Pounds = 68.04 Kilograms
 * 
 * Menu Choice (Valid):       3
 * Enter value (Invalid):     bad_input
 * Output:                    Not a valid number. Try again: 2.2
 * Output:                    2.20 Pounds = 1.00 Kilograms
 * 
 * Menu Input Validation Checks 
 * Enter your choice (1-4):   hello
 * Output:                    Not a valid option. Try again: 99
 * Output:                    Number out of range. Choose 1 to 4.
 * Enter your choice (1-4):   4
 * Output:                    Goodbye!
 */

