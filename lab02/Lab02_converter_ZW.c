/*
* Author: Zachary Wertz
* Exercise: Lab02
* Description: This lab is a unit conversions to get us introduced into
* the ideas of printf and scanf functions
*/

#include <stdio.h>


//------- Unit conversion & Variable Decloration -------//

double fahrenheit_to_celsius(double f) {
     return (f-32)*( 5.0/9.0 );
}

double pounds_to_kilograms(double k) {
    return (k*0.46536);
}

double pounds_to_tons(double t) {
    return (t / 2000 );
}

//------- Clearing the Variables -------//

void clear_input_buffer(void) {
   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF){ }
}

//------------- Program -------------//
int main(void){
int choice = 0;
int user_input = 0;
int intger_val = 0;
double user_input_double = 0;
double double_val = 0;

    while(1) {
	printf("------ Main Menu ------\n");
	printf("----- Enter (1-5) ------\n");
	printf("1: Fahrenheit to Celsius\n");
	printf("2: Pounds to Kilograms\n");
	printf("3: Pounds to Tons\n");
	printf("4: Intger math Example\n");
	printf("5: Exit\n");

if (scanf("%d", &choice) !=1) {
	printf("Invalid Input! Please enter a number (1-5)\n");
	clear_input_buffer();
	continue;
}

if (choice == 4) {
	printf("Please enter a number. (1, 2 ...)\n");
	scanf("%d", &user_input);
	intger_val =(user_input / 2);
	printf(" The value %d when used with intgers will become %d from %d/2.\n\n\n",user_input, intger_val , user_input);
	printf("Please enter a partial number. (1.0, 2.3 ...)\n\n");
	scanf("%lf", &user_input_double);
	double_val = (user_input_double / 2.0);
	printf(" The value %lf when used with doubles will become %lf from %lf /2\n\n\n",user_input_double, double_val , user_input_double);//test//
	continue;
}

if (choice == 5) {
	printf("Goodbye, Thank you for Converting.\n");
	break;
}
double input_value = 0.0;

printf("Enter a value to convert: ");

// Checks the input Value //
if(scanf("%lf", &input_value) !=1) {
	printf("invalid Input! Try again.\n");
	clear_input_buffer();
	continue;
}

double result = 0.0;

switch (choice) {
	case 1:
	result = fahrenheit_to_celsius(input_value);
	printf("%.2f Fahrenheit = %.2f Celsius\n\n", input_value,result);
	break;

	case 2:
	result = pounds_to_kilograms(input_value);
	printf("%.2f Pounds = %.2f Kilograms\n\n", input_value,result);
	break;

	case 3:
	result = pounds_to_tons(input_value);
	printf("%.2f Pounds = %.2f Tons\n\n",input_value,result);
	break;

	default:
	printf("Invalid menu choice! Try again.\n\n");

	}
}
return 0;
}
