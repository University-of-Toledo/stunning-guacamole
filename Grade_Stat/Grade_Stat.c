/**
 * @author Zachary Wertz
 * @exercise CSET 3150 - Lab 4: Grade Statistics
 * @description Reads a user-defined series of grades
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_GRADES 100

// Function prototypes
void demonstrate_array_decay(const double grades[], int count);
double compute_mean(const double *grades, int count);
double compute_min(const double *grades, int count);
double compute_max(const double *grades, int count);
void sort_grades(double *grades, int count);
double compute_median(const double *sorted_grades, int count);

int main(void) {
    double grades[MAX_GRADES];
    int count = 0;
    int i;

    // Prompt for the number of grades with input validation
    printf("Enter the number of grades to process (1 to %d): ", MAX_GRADES);
    while (scanf("%d", &count) != 1 || count <= 0 || count > MAX_GRADES) {
        // Clear invalid input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        printf("Invalid input. Please enter a valid integer between 1 and %d: ", MAX_GRADES);
    }

    // Read and validate individual grade values
    printf("Enter %d grade values (0.0 to 100.0):\n", count);
    for (i = 0; i < count; i++) {
        printf("Grade %d: ", i + 1);
        while (scanf("%lf", &grades[i]) != 1 || grades[i] < 0.0 || grades[i] > 100.0) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid grade. Enter a value between 0.0 and 100.0 for Grade %d: ", i + 1);
        }
    }

    // Array decay loss demonstration
    printf("\n--- Array Decay Demonstration ---\n");
    printf("Size of array in main (declared here): %zu bytes\n", sizeof(grades));
    demonstrate_array_decay(grades, count);

    // Compute statistics for the grades
    double mean = compute_mean(grades, count);
    double min = compute_min(grades, count);
    double max = compute_max(grades, count);

    // Sorting a copy or sorting in place for median calculation
    // (Passing the original array to sort_grades will modify it in place)
    sort_grades(grades, count);
    double median = compute_median(grades, count);

    // Print a formatted statistics report
    printf("\n========================================\n");
    printf("           GRADE STATISTICS REPORT      \n");
    printf("========================================\n");
    printf(" Total Grades Processed : %d\n", count);
    printf(" Minimum Grade          : %.2f\n", min);
    printf(" Maximum Grade          : %.2f\n", max);
    printf(" Mean (Average)         : %.2f\n", mean);
    printf(" Median                 : %.2f\n", median);
    printf("========================================\n");

    return 0;
}

/**
 * Demonstrates why sizeof behaves differently when an array is passed to a function.
 */
void demonstrate_array_decay(const double grades[], int count) {
    (void)count; // Suppress unused parameter warning if needed
    // Inside this function, 'grades' has decayed from an array to a pointer (const double *).
    // Therefore, sizeof(grades) returns the size of the pointer itself (e.g., 8 bytes on 64-bit systems),
    // rather than the total size of the allocated array (800 bytes for 100 doubles).
    printf("Size of parameter in function scope : %zu bytes\n", sizeof(grades));
    printf("Explanation: The array decayed to a pointer upon function call, losing its size metadata.\n");
}

/**
 * Computes the mean of the grades.
 */
double compute_mean(const double *grades, int count) {
    if (count <= 0) return 0.0;
    double sum = 0.0;
    int i;
    for (i = 0; i < count; i++) {
        sum += grades[i];
    }
    return sum / count;
}

/**
 * Finds the minimum grade.
 */
double compute_min(const double *grades, int count) {
    if (count <= 0) return 0.0;
    double min_val = grades[0];
    int i;
    for (i = 1; i < count; i++) {
        if (grades[i] < min_val) {
            min_val = grades[i];
        }
    }
    return min_val;
}

/**
 * Finds the maximum grade.
 */
double compute_max(const double *grades, int count) {
    if (count <= 0) return 0.0;
    double max_val = grades[0];
    int i;
    for (i = 1; i < count; i++) {
        if (grades[i] > max_val) {
            max_val = grades[i];
        }
    }
    return max_val;
}

/**
 * Sorts the array in place using a sorting algorithm.
 */
void sort_grades(double *grades, int count) {
    int i, j;
    double key;
    for (i = 1; i < count; i++) {
        key = grades[i];
        j = i - 1;
        while (j >= 0 && grades[j] > key) {
            grades[j + 1] = grades[j];
            j = j - 1;
        }
        grades[j + 1] = key;
    }
}

/**
 * Computes the median from a pre-sorted array.
 */
double compute_median(const double *sorted_grades, int count) {
    if (count <= 0) return 0.0;
    
    if (count % 2 != 0) {
        // Odd count: return the middle element
        return sorted_grades[count / 2];
    } else {
        // Even count: return the average of the two middle elements
        int mid1 = count / 2 - 1;
        int mid2 = count / 2;
        return (sorted_grades[mid1] + sorted_grades[mid2]) / 2.0;
    }
}
