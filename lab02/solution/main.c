/*
 * Author: Joseph Duvall
 * Exercise: Lab 2 Unit converter
 * Description: Converts temperatures, miles, and pounds
 */

#include <stdio.h>

int main(){

    float c, f;
    float miles, kilometers;
    float pounds, kilograms;

    printf("\nEnter the temperature in the Fahrenheit scale: ");
    scanf("%f", &f);
    c = (f - 32) * 5 / 9;
    printf("\nThe temperature in Celsius: %.2f", c);

    printf("\n\nEnter the distance in miles: ");
    scanf("%f", &miles);
    kilometers = miles * 1.60934;
    printf("\nThe distance in kilometers: %.2f", kilometers);

    printf("\n\nEnter the weight in pounds: ");
    scanf("%f", &pounds);
    kilograms = pounds * 0.453592;
    printf("\nThe weight in kilograms: %.2f", kilograms);

    return 0;
}
