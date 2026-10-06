/* Author Connor Rademaker
 * converter.c
 * CSET 3150 - Lab 2 
 * Holds the unit conversions
 */

#include <stdio.h>
#include "converter.h"

int main()
{
    float a = 0;
    float b = 0;
    float c = 0;
    float answer = 0;
    int choice =0;
    
    
    while(choice != 5)
    {
        printf("Choose what conversion you want (Input 1, 2, 3, 4, 5)\n Feet to meter 1\n Ounces to litres 2\n Pounds to kilograms 3\n Show the difference between integer division and double division 4\n Quit 5\n\n");
        
        if(scanf("%d", &choice) != 1)
        {
            printf("Enter a valid input (1, 2, 3, 4, 5)\n\n");
            
            while(getchar() != '\n');
            continue;
        }    
        
        if(choice == 1)
        {
            printf("Input feet\n");
            scanf("%f", &a);
            answer = feet_meter(a);
            printf("%.2f meters\n\n" , answer);
            
        }
        if(choice == 2)
        {    
            printf("Input ounces\n");
            scanf("%f", &b);
            answer = ounce_litre(b);
            printf("%.2f litres\n\n" , answer);
            
        }
        if(choice == 3)
        {
            printf("Input pounds\n");
            scanf("%f", &c);
            answer = lbs_kilo(c);
            printf("%.2f kilograms\n\n" , answer);
        }
        
        if(choice == 4)
        {
            int intdiv = (7/2);
            double z = 7;
            double y = 2;
            double doublediv = z / y;
            printf("This is integer division %d\n\n" , intdiv);
            printf("This is double division %.2f\n\n" , doublediv);
        }
            
    }
}