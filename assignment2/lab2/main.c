/* Author Connor Rademaker
 * main.c
 * CSET 3150 - Lab 2 
 * Main file containing the code for the program
 */

#include <stdio.h>
#include "converter.h"

int main()
{
    /*initializing variables used for holding inputs*/
    float a = 0;
    float b = 0;
    float c = 0;
    float answer = 0;
    int choice =0;
    
    /*main loop that will repeat till the user inputs 5*/
    while(choice != 5)
    {
        /*prints the first menu*/
        printf("Choose what conversion you want (Input 1, 2, 3, 4, 5).\n1. Feet to meters \n2. Ounces to litres \n3. Pounds to kilograms \n4. Show the difference between integer division and double division \n5. Quit \n\n");
        
        /*takes user input will continue through if statement, if the variable is a char or greater than 5*/
        if((scanf("%d", &choice) != 1) || choice > 5)
        {
            printf("Enter a valid input (1, 2, 3, 4, 5)\n\n");
            
            /*clears the input of all of the values in it until user hits enter, 
             *then it ends the loop and continue moves the code back to the start
             */
            while(getchar() != '\n');
            continue;
        }    
        
        /*feet to meter conversion function call*/
        if(choice == 1)
        {
            printf("Input feet\n");
            scanf("%f", &a);
            answer = feet_meter(a);
            printf("%.2f meters\n\n" , answer);
            
        }
        
        /*ounce to litre conversion function call*/        
        if(choice == 2)
        {    
            printf("Input ounces\n");
            scanf("%f", &b);
            answer = ounce_litre(b);
            printf("%.2f litres\n\n" , answer);
            
        }
        
        /*pound to kilogram conversion function call*/
        if(choice == 3)
        {
            printf("Input pounds\n");
            scanf("%f", &c);
            answer = lbs_kilo(c);
            printf("%.2f kilograms\n\n" , answer);
        }
        
        /*displays how division is done by the compiler when it uses doubles or integers*/
        if(choice == 4)
        {
            int intdiv = (7/2);
            double z = 7;
            double y = 2;
            double doublediv = z / y;
            printf("This is integer division using 7/2 = %d\n\n" , intdiv);
            printf("This is double division 7/2 = %.2f\n\n" , doublediv);
        }
            
    }
}