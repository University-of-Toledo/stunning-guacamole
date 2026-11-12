/*
 Name: Fiza Shaikh
 Lab 4 - Grade Statistics
 Description: Reads up to 100 grades as doubles,sorts the grades, and prints the min, max, mean, and median.
*/

#include <stdio.h>
#define MAX_GRADES 100

double compute_mean(const double *grades, int count);
double compute_min(const double *grades, int count);
double compute_max(const double *grades, int count);
void sort_grades(double *grades, int count);
double compute_median(const double *sorted_grades, int count);
void clear_line(void);

int main(void) {
    double grades[MAX_GRADES];
    int count = 0;
    int valid = 0;

    while (!valid) {
        printf("Enter number of Grades (1-%d)? ", MAX_GRADES);
        int result = scanf("%d", &count);

        if (result == EOF) {
            return 1;
        } else if (result != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_line();
        } else if (count < 1 || count > MAX_GRADES) {
            printf("Please enter a count between 1 and %d.\n", MAX_GRADES);
            clear_line();
        } else {
            clear_line();
            valid = 1;
        }
    }

    for (int i = 0; i < count; i++) {
        valid = 0;
        while (!valid) {
            double grade = 0.0;
            printf("Enter grade %d (0-100): ", i + 1);
            int result = scanf("%lf", &grade);

            if (result == EOF) {
                return 1;
            } else if (result != 1) {
                printf("Invalid input. Please enter a number.\n");
                clear_line();
            } else if (grade < 0.0 || grade > 100.0) {
                printf("Grade must be between 0 and 100.\n");
                clear_line();
            } else {
                grades[i] = grade;
                clear_line();
                valid = 1;
            }
        }
    }

    /*
     sizeof grades array explanation:
     In main, grades is declared as an array of 100 doubles, so sizeof(grades)
     gives the size of the whole array in bytes (100 * 8 = 800 bytes).
     When grades is passed to compute_mean, it decays into a pointer to its
     first element, so sizeof(grades) inside that function only gives the
     size of a pointer (8 bytes on a 64-bit system), not the whole array.
     */
    printf("\nsizeof(grades) in main: %zu bytes\n", sizeof(grades));

    double min = compute_min(grades, count);
    double max = compute_max(grades, count);
    double mean = compute_mean(grades, count);

    sort_grades(grades, count);
    double median = compute_median(grades, count);

    printf("\nGrade Statistics:\n");
    printf("Count:  %d\n", count);
    printf("Min:    %.2f\n", min);
    printf("Max:    %.2f\n", max);
    printf("Mean:   %.2f\n", mean);
    printf("Median: %.2f\n", median);

    return 0;
}

void clear_line(void) {
    int c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

double compute_mean(const double *grades, int count) {
    printf("sizeof(grades) in compute_mean: %zu bytes\n", sizeof(grades));

    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum = sum + grades[i];
    }
    return sum / count;
}

double compute_min(const double *grades, int count) {
    double min = grades[0];
    for (int i = 1; i < count; i++) {
        if (grades[i] < min) {
            min = grades[i];
        }
    }
    return min;
}

double compute_max(const double *grades, int count) {
    double max = grades[0];
    for (int i = 1; i < count; i++) {
        if (grades[i] > max) {
            max = grades[i];
        }
    }
    return max;
}

void sort_grades(double *grades, int count) {
    for (int i = 0; i < count - 1; i++) {
        int smallest = i;
        for (int j = i + 1; j < count; j++) {
            if (grades[j] < grades[smallest]) {
                smallest = j;
            }
        }
        double temp = grades[i];
        grades[i] = grades[smallest];
        grades[smallest] = temp;
    }
}

double compute_median(const double *sorted_grades, int count) {
    int middle = count / 2;
    if (count % 2 == 1) {
        return sorted_grades[middle];
    } else {
        return (sorted_grades[middle - 1] + sorted_grades[middle]) / 2.0;
    }
}

/*
Test Runs:

Odd-Count Test Run
Enter number of grades (1-100)? 5
Enter grade 1 (0-100): 85
Enter grade 2 (0-100): 92
Enter grade 3 (0-100): 78
Enter grade 4 (0-100): 90
Enter grade 5 (0-100): 88

sizeof(grades) in main: 800 bytes
sizeof(grades) in compute_mean: 8 bytes

Grade Statistics:
Count:  5
Min:    78.00
Max:    92.00
Mean:   86.60
Median: 88.00

Even-Count Test Run
Enter number of grades (1-100)? 4
Enter grade 1 (0-100): 70
Enter grade 2 (0-100): 80
Enter grade 3 (0-100): 90
Enter grade 4 (0-100): 100

sizeof(grades) in main: 800 bytes
sizeof(grades) in compute_mean: 8 bytes

Grade Statistics:
Count:  4
Min:    70.00
Max:    100.00
Mean:   85.00
Median: 85.00
*/
