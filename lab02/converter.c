#include "converter.h"

/*
Conversion formulas are used below with the variables from main
*/

double fahrenheit_celcius(double fahrenhiet)
    {
        return ( (fahrenhiet - 32) / 1.8);
    }

double kilometer_mile(double kilometers)
    {
        return (kilometers * 0.62137119);
    }

double pounds_kilo(double pounds)
    {
        return (pounds * 0.4535924);
    }
