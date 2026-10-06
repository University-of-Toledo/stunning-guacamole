/* Author Connor Rademaker
 * converter.c
 * CSET 3150 - Lab 2 
 * Holds the unit conversions
 */
 
#include "converter.h"

/*This is the logic that each function uses and will multiply 
 *the variables from main.c and then pass the value back to main.c
 */
float feet_meter(float a)
{
    return (a * 0.3048);
}

float ounce_litre(float b)
{
    return (b * 0.02957353);
}

float lbs_kilo(float c)
{
    return (c * 0.4535924);
}