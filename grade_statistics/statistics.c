#include <stdio.h>
#define MAX_GRADES 100
static void clear_input_buffer(void) {
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {
	}
}
double compute_mean(const double *grades, int count) {
	double sum = 0.0;
	for (int i = 0; i < count; i++) {
		sum += grades[i];
	}
	return sum / (double)count;
}
double compute_min(const double *grades, int count) {
    double min_val = grades[0];
    for (int i = 1; i < count; i++) {
        if (grades[i] < min_val) {
            min_val = grades[i];
        }
    }
    return min_val;
}

double compute_max(const double *grades, int count) {
    double max_val = grades[0];
    for (int i = 1; i < count; i++) {
        if (grades[i] > max_val) {
            max_val = grades[i];
        }
    }
    return max_val;
}

void sort_grades(double *grades, int count) {
    for (int i = 0; i < count - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < count; j++) {
            if (grades[j] < grades[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            double temp = grades[i];
            grades[i] = grades[min_idx];
            grades[min_idx] = temp;
        }
    }
}

double compute_median(const double *sorted_grades, int count) {
    if (count % 2 == 1) {
        return sorted_grades[count / 2];
    }
    return (sorted_grades[(count / 2) - 1] + sorted_grades[count / 2]) / 2.0;
}

static void show_sizeof_decay(const double *grades) {
    printf("sizeof(grades) inside function: %zu\n", sizeof(grades));
}

int main(void) {
    double grades[MAX_GRADES];
    int count = 0;

    while (1) {
        printf("Enter number of grades (1-%d): ", MAX_GRADES);
        if (scanf("%d", &count) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (count <= 0 || count > MAX_GRADES) {
            printf("Count must be between 1 and %d.\n", MAX_GRADES);
            continue;
        }
        break;
    }

    for (int i = 0; i < count; i++) {
        while (1) {
            double val = 0.0;
            printf("Enter grade %d (0-100): ", i + 1);
            if (scanf("%lf", &val) != 1) {
                printf("Invalid input. Please enter a number.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();

            if (val < 0.0 || val > 100.0) {
                printf("Grade must be between 0 and 100.\n");
                continue;
            }
            grades[i] = val;
            break;
        }
    }

    printf("sizeof(grades) in main: %zu\n", sizeof(grades));
    show_sizeof_decay(grades);

    double min_val = compute_min(grades, count);
    double max_val = compute_max(grades, count);
    double mean_val = compute_mean(grades, count);

    sort_grades(grades, count);
    double median_val = compute_median(grades, count);

    printf("\nGrade Statistics Report\n");
    printf("Count : %d\n", count);
    printf("Min : %.2f\n", min_val);
    printf("Max : %.2f\n", max_val);
    printf("Mean : %.2f\n", mean_val);
    printf("Median: %.2f\n", median_val);

    return 0;
}

