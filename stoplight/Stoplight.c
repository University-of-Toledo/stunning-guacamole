/* Author Connor Rademaker
 * Stoplight.c
 * CSET 3150 - HW Stoplight
 * Contains the stoplight code
 */

#include <wiringPi.h> /*Included library for wiring*/
#include <stdio.h> /*include for printf and scanf*/

#define RED 0 // pins the code uses
#define YELLOW 2 // 0 is pin 11, 2 is pin 13, 3 is pin 15
#define GREEN 3

int main(void)
{ 
	wiringPiSetup(); /* initalizes the GPIO pins*/
	
	pinMode(0,OUTPUT); /*Changes the voltage on the pin as an output*/
	pinMode(2,OUTPUT); //Sets pins for GPIO
	pinMode(3,OUTPUT);
	
	digitalWrite(RED, LOW); /*Ensures the leds are off*/
	digitalWrite(YELLOW, LOW);
	digitalWrite(GREEN, LOW);
	
	int loops = 0;
	

	
	printf("Choose amount of times to run the loop: ");
	if(scanf("%d", &loops) <= 0)
        {
            printf("Enter a valid input (1, 2, 3...)\n\n");
            
            /*clears the input of all of the values in it until user hits enter, 
             *then it ends the loop and continue moves the code back to the start
             */
            while(getchar() != '\n');
        }   
	
	
	
        
	while (loops > 0) //Till amount of loops is out
	{
          
		digitalWrite(RED, HIGH); //On
		delay(1000); // 1000ms
		digitalWrite(RED, LOW); //off
		delay(200);// 200ms
		
		digitalWrite(GREEN, HIGH); //on
		delay(2000); //2000ms
		digitalWrite(GREEN, LOW); //off
		delay(200); //200ms
		
		digitalWrite(YELLOW, HIGH); //on
		delay(1000); //1000ms
		digitalWrite(YELLOW, LOW); //off
		delay(200); //200ms
		
		printf("Loops left: %d\n", (loops - 1)); //tells how many loops left
		
		
		loops--; //increments loops down one
	
	} 
	
	return 0;
}
