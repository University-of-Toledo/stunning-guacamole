/* Author Connor Rademaker
 * converter.c
 * CSET 3150 - Lab 2 
 * Holds the unit conversions
 */
 
#include "converter.h"

/*This is the logic that each function uses and will multiply 
 *the variables from main.c and then pass the value back to main.c
 */
double feet_meter(double feet)
{
    return (feet * 0.3048);
}

double ounce_litre(double ounces)
{
    return (ounces * 0.02957353);
}

double lbs_kilo(double lbs)
{
    return (lbs * 0.4535924);
}