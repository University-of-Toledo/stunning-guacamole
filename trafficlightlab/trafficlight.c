
/*
 * blink.c
 *By Jeremias Ortiz
 *This lab adds 3 LEDs to the input/output to string as a traffic light.
 *The cycle lasts 3 times before auto shutoff.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wiringPi.h>


#define	RED 0  // RED LIGHT GPIO 17
#define YELLOW 2  // YELLOW LIGHT GPIO 27
#define GREEN 3  // GREEN LIGHT GPIO 22

int main (void){


  wiringPiSetup() ; // initialize wiringPi
  
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  for (int i = 0; i < 3; i++) 
     {

        digitalWrite(GREEN, HIGH);	// On
        delay(4000);	// mS
        digitalWrite(GREEN, LOW);	// Off;
        delay(100);	// mS
        
    
        digitalWrite(YELLOW, HIGH);	// On
        delay(3000);	// mS
        digitalWrite(YELLOW, LOW);	// Off;
        delay(100);	// mS
        
        
        digitalWrite(RED, HIGH);	// On
        delay(3000);	// mS
        digitalWrite(RED, LOW);	// Off;
        delay(100);	// mS
                
    }
  return 0 ;
}
