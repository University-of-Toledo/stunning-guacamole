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

	printf("How many grades do you want to enter? ");

	if (scanf("%d", &count) != 1)
	{
		printf("Invalid input. Please enter a number.\n");
		return 1;
	}

	if (count <= 0 || count > MAX_GRADES)
	{
		printf("Number of grades must be between 1 and %d.\n",
		       MAX_GRADES);
		return 1;
	}

	printf("\nEnter %d grades (0-100):\n", count);

	for (i = 0; i < count; i++)
	{
		printf("Enter grade %d: ", i + 1);

		if (scanf("%lf", &grades[i]) != 1)
		{
			printf("\nInvalid input. Please enter a number.\n");
			return 1;
		}

		if (grades[i] < 0 || grades[i] > 100)
		{
			printf("\nGrade must be between 0 and 100.\n");
			return 1;
		}
	}

	// size of complete array in the main function

	printf("\nArray size in main: %zu bytes\n", sizeof(grades));


	sort_grades(grades, count);


	printf("\nGrade Report\n");
	printf("Number of grades: %d\n", count);
	printf("Minimum grade:    %.2f\n", compute_min(grades, count));
	printf("Maximum grade:    %.2f\n", compute_max(grades, count));
	printf("Mean grade:       %.2f\n", compute_mean(grades, count));
	printf("Median grade:     %.2f\n", compute_median(grades, count));

	return 0;
}

double compute_mean(const double *grades, int count)
{
	double total = 0.0;
	int i;

	// size of pointer not the full array
	// array becomes a pointer once passed in a function

	printf("Array size inside function: %zu bytes\n",
	       sizeof(grades));

	for (i = 0; i < count; i++)
	{
		total += grades[i];
	}

	return total / count;
}

double compute_min(const double *grades, int count)
{
	double minimum = grades[0];
	int i;

	for (i = 1; i < count; i++)
	{
		if (grades[i] < minimum)
		{
			minimum = grades[i];
		}
	}

	return minimum;
}

double compute_max(const double *grades, int count)
{
	double maximum = grades[0];
	int i;

	for (i = 1; i < count; i++)
	{
		if (grades[i] > maximum)
		{
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

	for (i = 0; i < count - 1; i++)
	{
		min_index = i;

		for (j = i + 1; j < count; j++)
		{
			if (grades[j] < grades[min_index])
			{
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
	if (count % 2 == 1)
	{
		return sorted_grades[count / 2];
	}
	else
	{
		return (sorted_grades[count / 2 - 1] +
		        sorted_grades[count / 2]) / 2.0;
	}
}
