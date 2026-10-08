/* Made by Jeremias Ortiz
 * Converter operations between U.S. and metric units.
 */
#include <stdio.h> 
#include "converterunit.h"


double mile_kilometer(double mile)
{
    return (&inputmile * 1.609);
}

double celcius_farenheit(double celcius)
{
    return ((&inputC * 1.8) + 32);
}

double kilogram_lbs(double kilogram)
{
    return (&inputkg / 2.205);
}
