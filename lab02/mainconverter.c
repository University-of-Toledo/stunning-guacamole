#include <stdio.h>

int main() {
	char category;
	double inputC;
	double inputmile;
	double inputkg;
	double celcius_farenheit;
	double mile_kilometer;
	double kilogram_lbs;

	
  printf("Welcome to Unit Converter! \n");
  printf("Please enter Temperature (T),Distance (D),Mass (M) \n");
  printf("Please enter the letter you want to convert.\n");
  scanf(" %c",&category);

	if(category == 'T'){
		printf("Welcome to celcius to farenheit converter! \n");
			printf("Please enter the celcius degree: \n");
			scanf(" %lf", &inputC);
			celcius_farenheit = ((inputC * 1.80) + 32);
			printf("farenheit: %lf", celcius_farenheit);
		}
		
	if(category == 'D'){
		printf("Welcome to mile to kilometer converter! \n");
			printf("Please enter the mile: ");
			scanf(" %lf", &inputmile);
			mile_kilometer = (inputmile * 1.609);
			printf("kilometer: %lf", mile_kilometer);
		}		
	if(category == 'M'){
		printf("Welcome to kilogram to pound converter! \n");
			printf("Please enter the kilogram: \n");
			scanf(" %lf", &inputkg);
			kilogram_lbs = (inputkg * 2.205);
			printf("pound: %lf", kilogram_lbs);
		}		
    return 0;
}
