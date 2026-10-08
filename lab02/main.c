// author : Mike 
// Lab 2 
// how to convert from F to C, pounds to kg, and meters to feet

#include <stdio.h>

int main(){
	
	float f,c;
	
	printf("\n Temp in F");
	
	scanf("%f",&f);
	
	c = (f-32) * 5/9;
	
	printf("\n Temp in C : %f" ,c);
	
	
	float p,kg;
	
	printf("\n weight in p");
	
	scanf("%f",&p);
	
	kg = p*0.54359237;
	
	printf("\n weight in kg : %f" ,kg);
	
	float m,ft;
	
	printf("\n distance in m");
	
	scanf("%f",&m);
	
	ft = m*3.28084;
	
	printf("\n distance in ft : %f" ,ft);
	
	return 0;
}
