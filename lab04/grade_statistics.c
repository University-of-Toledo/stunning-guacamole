
/*
Author: Ozzy Finnegan
Lab 04: Grade Statistics
Description: Calculate the minimum, maximum, mean,
and median of student grades.
*/

#include <stdio.h>

#define MAX_GRADES 100

double compute_mean(const double *grades, int count);
double compute_min(const double *grades, int count);
double compute_max(const double *grades, int count);
void sort_grades(double *grades, int count);
double compute_median(const double *sorted_grades, int count);
void demonstrate_sizeof(const double grades[], int count);

int main(void)
{
    double grades[MAX_GRADES];
    int count;

    printf("Grade Statistics Program\n");
    printf("------------------------\n");

    printf("How many grades would you like to enter (1-100)? ");

    if (scanf("%d", &count) != 1 ||
        count < 1 || count > MAX_GRADES)
    {
        printf("Invalid number of grades.\n");
        return 1;
    }

    for (int i = 0; i < count; i++)
    {
        printf("Enter grade %d (0-100): ", i + 1);

        if (scanf("%lf", &grades[i]) != 1 ||
            grades[i] < 0.0 || grades[i] > 100.0)
        {
            printf("Invalid grade entered.\n");
            return 1;
        }
    }

    printf("\nGrades entered:\n");

    for (int i = 0; i < count; i++)
    {
        printf("%.2f\n", grades[i]);
    }

    double mean = compute_mean(grades, count);
    double minimum = compute_min(grades, count);
    double maximum = compute_max(grades, count);

    printf("\nArray size in main: %zu bytes\n",
           sizeof(grades));

    printf("Size of one element: %zu bytes\n",
           sizeof(grades[0]));

    printf("Array capacity: %zu elements\n",
           sizeof(grades) / sizeof(grades[0]));

    demonstrate_sizeof(grades, count);

    sort_grades(grades, count);

    printf("\nSorted grades:\n");

    for (int i = 0; i < count; i++)
    {
        printf("%.2f\n", grades[i]);
    }

    double median = compute_median(grades, count);

    printf("\nGrade Statistics Results\n");
    printf("------------------------\n");
    printf("Number of grades: %d\n", count);
    printf("Minimum: %.2f\n", minimum);
    printf("Maximum: %.2f\n", maximum);
    printf("Mean: %.2f\n", mean);
    printf("Median: %.2f\n", median);

    return 0;
}

double compute_mean(const double *grades, int count)
{
    double sum = 0.0;

    for (int i = 0; i < count; i++)
    {
        sum += grades[i];
    }

    return sum / count;
}

double compute_min(const double *grades, int count)
{
    double minimum = grades[0];

    for (int i = 1; i < count; i++)
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

    for (int i = 1; i < count; i++)
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
    for (int i = 0; i < count - 1; i++)
    {
        int min_index = i;

        for (int j = i + 1; j < count; j++)
        {
            if (grades[j] < grades[min_index])
            {
                min_index = j;
            }
        }

        if (min_index != i)
        {
            double temp = grades[i];
            grades[i] = grades[min_index];
            grades[min_index] = temp;
        }
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
        int middle = count / 2;

        return (sorted_grades[middle - 1] +
                sorted_grades[middle]) / 2.0;
    }
}

void demonstrate_sizeof(const double grades[], int count)
{
    const double *pointer = grades;

    printf("\nInside demonstrate_sizeof():\n");

    printf("Pointer size: %zu bytes\n",
           sizeof(pointer));

    printf("Number of grades passed: %d\n", count);
}
