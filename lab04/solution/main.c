#include <stdio.h>

#define MAX_GRADES 100

double find_mean(double grades[], int count);
double find_min(double grades[], int count);
double find_max(double grades[], int count);
void sort_grades(double grades[], int count);
double find_median(double grades[], int count);

int main(void)
{
    double grades[MAX_GRADES];
    int count;

    printf("Enter number of grades: ");

    if (scanf("%d", &count) != 1)
    {
        printf("Invalid input.\n");
        return 1;
    }

    if (count < 1 || count > MAX_GRADES)
    {
        printf("Invalid number of grades.\n");
        return 1;
    }

    // Get grades from the user
    for (int i = 0; i < count; i++)
    {
        printf("Enter grade %d: ", i + 1);

        if (scanf("%lf", &grades[i]) != 1)
        {
            printf("Invalid input.\n");
            return 1;
        }

        if (grades[i] < 0 || grades[i] > 100)
        {
            printf("Grade must be between 0 and 100.\n");
            return 1;
        }
    }

    // Show the size of the array
    printf("\nArray size in main: %zu bytes\n", sizeof(grades));

    sort_grades(grades, count);

    printf("\nGrade Results\n");
    printf("Count: %d\n", count);
    printf("Minimum: %.2f\n", find_min(grades, count));
    printf("Maximum: %.2f\n", find_max(grades, count));
    printf("Mean: %.2f\n", find_mean(grades, count));
    printf("Median: %.2f\n", find_median(grades, count));

    return 0;
}

// Find the average
double find_mean(double grades[], int count)
{
    double total = 0;

    // An array passed to a function becomes a pointer
    printf("Array size in function: %zu bytes\n", sizeof(double *));

    for (int i = 0; i < count; i++)
    {
        total = total + grades[i];
    }

    return total / count;
}

// Find the lowest grade
double find_min(double grades[], int count)
{
    double lowest = grades[0];

    for (int i = 1; i < count; i++)
    {
        if (grades[i] < lowest)
        {
            lowest = grades[i];
        }
    }

    return lowest;
}

// Find the highest grade
double find_max(double grades[], int count)
{
    double highest = grades[0];

    for (int i = 1; i < count; i++)
    {
        if (grades[i] > highest)
        {
            highest = grades[i];
        }
    }

    return highest;
}

// Put grades in order from lowest to highest
void sort_grades(double grades[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (grades[i] > grades[j])
            {
                double temp = grades[i];
                grades[i] = grades[j];
                grades[j] = temp;
            }
        }
    }
}

// Find the middle grade
double find_median(double grades[], int count)
{
    if (count % 2 == 1)
    {
        return grades[count / 2];
    }
    else
    {
        return (grades[count / 2 - 1] + grades[count / 2]) / 2.0;
    }
}
