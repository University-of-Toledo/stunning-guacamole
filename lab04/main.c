/*
Author: Jonathan Rodriguez
Exercise: Lab04 - Grade Statistics
Description: Reads grades and calculates their minimum, maximum, mean, and median.
*/
#include <stdio.h>
#define MAX_GRADES 100 
double compute_mean(const double *grades, int count);
double compute_min(const double *grades, int count);
double compute_max(const double *grades, int count);
void sort_grades(double *grades, int count);
double compute_median(const double *sorted_grades, int count);

int main(void){
	double grades[MAX_GRADES]; // Stores up to 100 grades, including decimals.
	int count;
	int i;
	printf("Array size in main: %zu bytes\n", sizeof grades);
	printf("Enter number of grades: ");

	if (scanf("%d", &count) != 1){
		printf("Invalid number\n");
		return 1;
		}

	if (count <= 0 || count > MAX_GRADES){
		printf("Number of grades must be between 1 and %d\n", MAX_GRADES);
		return 1;
		}

	for (i = 0; i < count; i++){
		printf("Enter grade %d: ", i + 1);

		if (scanf("%lf", &grades[i]) != 1){ 
			printf("Invalid grade\n");
			return 1;
			}

		if (grades[i] < 0 || grades[i] > 100){
			printf("Grades must be between 0 and 100\n");
			return 1;
			}
		}
	sort_grades(grades, count);
	double mean = compute_mean(grades, count);
	printf("Count: %d\n", count);
	printf("Min: %.2f\n", compute_min(grades, count));
	printf("Max: %.2f\n", compute_max(grades, count));
	printf("Mean: %.2f\n", mean);
	printf("Median: %.2f\n", compute_median(grades, count));
	return 0;
	}

double compute_mean(const double *grades, int count){
	double sum = 0;
	int i;
	printf("Pointer size in compute_mean: %zu bytes\n", sizeof grades);// grades is a pointer here, sizeof measure it
	for (i = 0; i < count; i++) {
		sum = sum + grades[i];
		}
	return sum / count;
	}

double compute_min(const double *grades, int count){
	double min = grades[0];
	int i;

	for (i = 1; i < count; i++){
		if (grades[i] < min){
			min = grades[i];
			}
		}
	return min;
	}

double compute_max(const double *grades, int count){
	double max = grades[0];
	int i;

	for (i = 1; i < count; i++){

		if (grades[i] > max){
			max = grades[i];
			}
		}
	return max;
	}

void sort_grades(double *grades, int count){
	int i;
	int position;
	int minIndex;
	double temp;

	for (i = 0; i < count - 1; i++){
		minIndex = i;


		for (position = i + 1; position < count; position++){

			if (grades[position] < grades[minIndex]){
				minIndex = position;
				}
			}

		temp = grades[i];
		grades[i] = grades[minIndex];
		grades[minIndex] = temp;

		}
	}
double compute_median(const double *sorted_grades, int count){
	int middle;

	middle = count / 2;

	if (count % 2 == 0){
		return(sorted_grades[middle - 1] + sorted_grades[middle]) / 2.0;
		}
	return sorted_grades[middle];
	}
