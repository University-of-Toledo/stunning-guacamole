/* Author Connor Rademaker
 * blink.c
 * CSET 3150 - HW link
 * Contains the blinking code
 */

#include <wiringPi.h> /*Included library for wiring*/

int main(void)
{ 
	wiringPiSetup(); /* initalizes the GPIO pins*/
	
	pinMode(7,OUTPUT); /*Changes the voltage on the pin as an output*/
	
	
	while (1) /*loops endlessly*/
	{
		digitalWrite(7, HIGH); /*sets the pin to high, or 5V*/
		delay(1000); /* 1 sec delay*/
		
		digitalWrite(7, LOW); /*sets the pin to low, or 0V*/
		delay(1000); /* 1 sec delay*/
		
	} 
	
	return 0;
}
