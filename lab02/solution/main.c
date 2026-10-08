// Corbin Glambin
// Unit Converter
// Converts Farenheit to Celsius, Pouds to Kilograms, and Miles to Kilometers
#include <stdio.h>

int main()
{
    float Farenheit,Celsius;
    printf("Enter Temperature in Farenheit: ");
    scanf("%f",&Farenheit);
    Celsius=(Farenheit-32)*5/9;
    printf("\nTemperature in Celsius: %.3f",Celsius);
    
    float lb,kg;
    printf("\nEnter pounds: ");
    scanf("%f",&lb);
    kg=(lb*0.45359237);
    printf("\nWeight in kg: %.3f",kg);
    
    float mi,km;
    printf("\nEnter Miles: ");
    scanf("%f",&mi);
    km=(mi*1.609344);
    printf("\nDistance in km: %.3f",km);
    return 0;
}
