// Author: John Otto
// Course: CSET 3150
// Exercise: Lab 4 - Grade Statistics
// Description:
// Reads student grades into a fixed-size array,
// validates input, sorts grades, and calculates
// minimum, maximum, mean, and median.


#include <stdio.h>

#define MAX_GRADES 100

double compute_mean(const double *grades, int count);
double compute_min(const double *grades, int count);
double compute_max(const double *grades, int count);
void sort_grades(double *grades, int count);
double compute_median(const double *sorted_grades, int count);

int main(void)
{
	double grades[MAX_GRADES];
	int count;
	int i;

	printf("grade Statistics Program\n");
	printf("------------------------\n");

	// Get the number of grades
	printf("enter the number of grades (1-%d): ", MAX_GRADES);

	if (scanf("%d", &count) != 1) {
		printf("error: Invalid input. Enter an integer.\n");
		return 1;
	}

	if (count < 1 || count > MAX_GRADES) {
		printf("error: Number of grades must be between 1 and %d.\n",
		       MAX_GRADES);
		return 1;
	}

	// Read and validate grades
	for (i = 0; i < count; i++) {
		printf("enter grade %d (0-100): ", i + 1);

		if (scanf("%lf", &grades[i]) != 1) {
			printf("error: Invalid grade input.\n");
			return 1;
		}

		if (grades[i] < 0.0 || grades[i] > 100.0) {
			printf("error: Grade must be between 0 and 100.\n");
			return 1;
		}
	}


	printf("\nArray size demonstration:\n");
	printf("sizeof(grades) in main: %zu bytes\n", sizeof(grades));

	printf("sizeof(grades) in function: ");
	(void)compute_mean(grades, count);


	double minimum = compute_min(grades, count);
	double maximum = compute_max(grades, count);
	double mean = compute_mean(grades, count);

	// Sort grades before calculating median
	sort_grades(grades, count);

	double median = compute_median(grades, count);

	// Print sorted grades
	printf("\nSorted grades:\n");

	for (i = 0; i < count; i++) {
		printf("%.2f ", grades[i]);
	}

	// Print statistics report
	printf("\n\nGrade Statistics Report\n");
	printf("-----------------------\n");
	printf("Number of grades: %d\n", count);
	printf("Minimum:          %.2f\n", minimum);
	printf("Maximum:          %.2f\n", maximum);
	printf("Mean:             %.2f\n", mean);
	printf("Median:           %.2f\n", median);

	return 0;
}


double compute_mean(const double *grades, int count)
{
	double sum = 0.0;
	int i;

	//Demonstrate array decay
	printf("%zu bytes\n", sizeof(grades));

	for (i = 0; i < count; i++) {
		sum += grades[i];
	}

	return sum / count;
}


double compute_min(const double *grades, int count)
{
	double minimum = grades[0];
	int i;

	for (i = 1; i < count; i++) {
		if (grades[i] < minimum) {
			minimum = grades[i];
		}
	}

	return minimum;
}


double compute_max(const double *grades, int count)
{
	double maximum = grades[0];
	int i;

	for (i = 1; i < count; i++) {
		if (grades[i] > maximum) {
			maximum = grades[i];
		}
	}

	return maximum;
}


void sort_grades(double *grades, int count)
{
	int i;
	int j;
	int min_index;
	double temp;

	for (i = 0; i < count - 1; i++) {
		min_index = i;

		for (j = i + 1; j < count; j++) {
			if (grades[j] < grades[min_index]) {
				min_index = j;
			}
		}

		temp = grades[i];
		grades[i] = grades[min_index];
		grades[min_index] = temp;
	}
}


double compute_median(const double *sorted_grades, int count)
{
	if (count % 2 == 0) {
		return (sorted_grades[count / 2 - 1] +
		        sorted_grades[count / 2]) / 2.0;
	}

	return sorted_grades[count / 2];
}
