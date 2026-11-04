#include <stdio.h>

#define MAX_GRADES 100

void flush_line(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

double compute_mean(const double *grades, int count)
{
    double sum = 0;
    for (int i = 0; i < count; i++)
        sum += grades[i];
    return sum / count;
}

double compute_min(const double *grades, int count)
{
    double min = grades[0];
    for (int i = 1; i < count; i++)
        if (grades[i] < min)
            min = grades[i];
    return min;
}

double compute_max(const double *grades, int count)
{
    double max = grades[0];
    for (int i = 1; i < count; i++)
        if (grades[i] > max)
            max = grades[i];
    return max;
}

void sort_grades(double *grades, int count)
{
    for (int i = 1; i < count; i++) {
        double key = grades[i];
        int j = i - 1;
        while (j >= 0 && grades[j] > key) {
            grades[j + 1] = grades[j];
            j--;
        }
        grades[j + 1] = key;
    }
}

double compute_median(const double *sorted_grades, int count)
{
    if (count % 2)
        return sorted_grades[count / 2];
    return (sorted_grades[count / 2 - 1] + sorted_grades[count / 2]) / 2;
}

void show_size(const double *grades)
{
    printf("sizeof(grades) in function: %zu\n", sizeof(grades));
}

int main(void)
{
    double grades[MAX_GRADES];
    int count;

    printf("How many grades (1-%d)? ", MAX_GRADES);
    while (scanf("%d", &count) != 1 || count < 1 || count > MAX_GRADES) {
        if (feof(stdin))
            return 1;
        printf("Invalid count, try again: ");
        flush_line();
    }

    for (int i = 0; i < count;) {
        double g;
        printf("Grade %d (0-100): ", i + 1);
        if (scanf("%lf", &g) == 1 && g >= 0 && g <= 100) {
            grades[i++] = g;
            continue;
        }
        if (feof(stdin))
            return 1;
        printf("Invalid grade.\n");
        flush_line();
    }

    printf("\nsizeof(grades) in main: %zu\n", sizeof(grades));
    show_size(grades);

    sort_grades(grades, count);

    printf("\nCount : %d\n", count);
    printf("Min : %.2f\n", compute_min(grades, count));
    printf("Max : %.2f\n", compute_max(grades, count));
    printf("Mean : %.2f\n", compute_mean(grades, count));
    printf("Median: %.2f\n", compute_median(grades, count));
    return 0;
}
