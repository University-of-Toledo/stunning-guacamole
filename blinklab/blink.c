
/*
 * blink.c
 *By Jeremias Ortiz
 *This lab adds LED hardware to the input/output 
 *features of the Pi. An iteration counts 10 times before the LED shuts
 *off.
 */
#include <stdio.h>
#include <wiringPi.h>

#define	LED	0

int main (void)
{
  printf ("Raspberry Pi blink\n") ;

  wiringPiSetup () ;
  pinMode (LED, OUTPUT) ;

  for (int i = 0; i < 10; i++) 
  {
    digitalWrite (LED, HIGH) ;	// On
    delay (500) ;		// mS
    digitalWrite (LED, LOW) ;	// Off
    delay (500) ;		// mS
  }
  return 0 ;
}
