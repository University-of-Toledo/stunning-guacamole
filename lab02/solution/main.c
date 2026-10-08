// John Otto lab 2
// Unit Converter
// convert Fahrenheit to Celsius and kg to lb and feet to meter

#include <stdio.h>

int main(void)
{
	float f, c;
	printf ("template in fahrenheit:");
	scanf("%f", &f);

	c = (f - 32) * 5 / 9;

	printf("%.5f fahrenheit = %.5f celsius\n",f, c);
	
	
	float kg, lb;
	printf ("template in kg:");
	scanf("%f", &kg);

	lb = kg * 2.20462;

	printf("%.5f kg = %.5f lb\n",kg, lb);
	
		float ft, m;
	printf ("template in feet:");
	scanf("%f", &ft);

	m = ft * 0.3048;

	printf("%.5f feet = %.5f meter\n",ft, m);
	return 0;
}
