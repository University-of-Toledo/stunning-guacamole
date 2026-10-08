#include <stdio.h>

int main() {
	char category
	int inputC // User inputted celsius;
	float celcius_farenheit // variable that stores the C>F;
	
	int inputkg // User inputted kilogram;
	float kilogram_lbs // variable that stores the kg>lbs;
	
	int inputmile // User inputted mile;
	float mile_kilometer // variable that stores the mi>km;

	
  printf("Welcome to Unit Converter! \n");
  printf("Please enter Temperature(T),Distance(D),Mass(M) \n");
  printf("Please enter the letter you want to convert.\n");
  scanf("%c",&category);

	if(category == 'T'){
		printf("Welcome to celcius to farenheit converter! \n");
			printf("Please enter the celcius degree: \n");
			scanf("%d", &inputC);
			celcius_farenheit = ((&inputC + 32) * (5.0/9.0));
			printf("farenheit: %d", celcius_farenheit);
		}
    return 0;
}
