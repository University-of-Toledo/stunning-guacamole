/*
Author: Ozzy Finnegan
Lab 2: "Unit Converter"
Description: Converting units.
*/

#include <stdio.h>


double celsiustofahrenheit(double celsius)
{
    return (celsius * 9.0/5.0) + 32.0;
}
double milestokilometers(double miles)
{
    return (miles * 1.60934);
}
double poundstokilograms(double pounds)
{
    return (pounds * 0.453592);
}
double schrutebuckstousd(double schrutebucks)
{
    return(schrutebucks * 0.0001);
}

int main(void)
{
//Floating point demonstration.
int integerresult = 7 / 2;
double floatingresult = 7.0 / 2;

printf("Integer division: %d\n", integerresult);
printf("Floating point division: %.2f\n\n\n", floatingresult);

    int choice = -1;
    do
    {
        printf("     Unit Converter\n");
	printf("1. Celsius to Fahrenheit\n");
	printf("2. Miles to Kilometers\n");
	printf("3. Pounds to Kilograms\n");
	printf("4. Schrute Bucks to USD\n");
	printf("0. Exit\n\n");
	printf("Which unit would you like to convert?: ");

	if(scanf("%d", &choice) !=1)
	{
	printf("Invalid input. Please select a valid option.\n\n\n");
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
	choice = -1;
	continue;
    }

	switch (choice)
        {
	    case 1:
	    {
	    double celsius;
	    double fahrenheit;
	    printf("Enter temperature in Celsius: ");
	    scanf("%lf", &celsius);

	    fahrenheit = celsiustofahrenheit(celsius);
	    printf("%.2f Celsius = %.2f Fahrenheit \n\n\n", celsius, fahrenheit);

	    break;
	    }

	    case 2:
	    {
	    double miles;
	    double kilometers;
	    printf("Enter the distance in miles: ");
	    scanf("%lf", &miles);

	    kilometers = milestokilometers(miles);
	    printf("%.2f Miles = %.2f Kilometers\n\n\n", miles, kilometers);

	    break;
	    }

	    case 3:
	    {
	    double pounds;
	    double kilograms;
	    printf("Enter the amount in Pounds: ");
	    scanf("%lf", &pounds);

	    kilograms = poundstokilograms(pounds);
	    printf("%.2f Pounds = %.2f Kilograms\n\n\n", pounds, kilograms);

	    break;
	    }

	    case 4:
	    {
	    double schrutebucks;
	    double usd;
	    printf("Enter the amount of Schrute Bucks: ");
	    scanf("%lf", &schrutebucks);

	    usd = schrutebuckstousd(schrutebucks);
	    printf("%.2f Schrute Bucks = $%.2f USD\n\n\n", schrutebucks, usd);

	    break;
	    }

	    case 0:
	    printf("Goodbye.\n\n\n");
	    break;

	    default:
	    printf("Invalid input. Please select a valid option.\n\n\n");
	    break;
        }
    }
    while (choice != 0);

return 0;
}
