/*
Author: AJ Alexeev
Lab 2
Description: Converts various units into different ones based on user input
*/

#include <stdio.h>
#include "converter.h"

int main()
{

    double fahrenheit = 0;
    double kilometers = 0;
    double pounds = 0;
    double answer = 0;
    int choice = 0;
    
    while(choice != 5)
    {

        printf("Select a conversion:\n1. Fahrenheit to Celcius \n2. Kilometers to Miles \n3. Pounds to Kilograms \n4. Difference of integer division and double division \n5. End Program \n\n");
        
        if((scanf("%d", &choice) != 1))
        {
            printf("Invalid input, please use 1, 2, 3, 4, 5\n\n");
            /*
            
            */
            while (getchar() != '\n');
            continue;
        }    
        else if (choice < 1 || choice > 5)
        {
         printf("Invalid input, please use 1, 2, 3, 4, 5\n\n");
        }
        
        if(choice == 1)
        {
            printf("Input degrees Fahrenheit\n");
            scanf("%lf", &fahrenheit);
            answer = fahrenheit_celcius(fahrenheit);
            printf("%.2f Celcius\n\n" , answer);
        }
        
        if(choice == 2)
        {    
            printf("Input Kilometers\n");
            scanf("%lf", &kilometers);
            answer = kilometer_mile(kilometers);
            printf("%.2f Miles\n\n" , answer);
        }
        
        if(choice == 3)
        {
            printf("Input pounds\n");
            scanf("%lf", &pounds);
            answer = pounds_kilo(pounds);
            printf("%.2f Kilograms\n\n" , answer);
        }
        
        if(choice == 4)
        {
            int integerdivision = (7/2);
            
             double num = 7;
             double denom = 2;
                 double doubledivision = num / denom;
                 
            printf("Integer division with 7/2 = %d\n" , integerdivision);
            printf("Double division with 7/2 = %.2f\n\n" , doubledivision);
        }
         
    }
    
}
