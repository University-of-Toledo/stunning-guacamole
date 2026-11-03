/* Author Connor Rademaker
 * blink.c
 * CSET 3150 - HW link
 * Contains the blinking code
 */

#include <wiringPi.h> /*Included library for wiring*/

#define RED 11
#define YELLOW 13
#define GREEN 15

int main(void)
{ 
	wiringPiSetup(); /* initalizes the GPIO pins*/
	
	pinMode(11,OUTPUT); /*Changes the voltage on the pin as an output*/
	pinMode(13,OUTPUT); /* Red 11, Yellow 13, Green 15, GND 6*/
	pinMode(15,OUTPUT);
	
	digitalWrite(11, LOW); /*Ensures the leds are off*/
	digitalWrite(13, LOW);
	digitalWrite(15, LOW);
	
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
        
	while (loops > 0) /*Till amount of loops is out*/
	{
	    
	     
		digitalWrite(RED, HIGH); /*sets the pin to high, or 5V*/
		delay(1000); /* 1 sec delay*/
		digitalWrite(RED, LOW); /*sets the pin to low, or 0V*/
		delay(200);
		
		digitalWrite(GREEN, HIGH); /*sets the pin to high, or 5V*/
		delay(2000); /* 1 sec delay*/
		digitalWrite(GREEN, LOW); /*sets the pin to low, or 0V*/
		delay(200);
		
		digitalWrite(YELLOW, HIGH); /*sets the pin to high, or 5V*/
		delay(1000); /* 1 sec delay*/
		digitalWrite(11, LOW); /*sets the pin to low, or 0V*/
		delay(200);
		
		printf("Loops left: %d", (loops - 1));
		
		
		loops--;
	} 
	
	return 0;
}
