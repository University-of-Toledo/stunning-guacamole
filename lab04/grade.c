/* Author Connor Rademaker
 * grade.c
 * CSET 3150 - HW Grading Statistics
 * Contains the grading statistics code
 */
#include <stdio.h>

#define MAX_GRADES 100

//all the functions
double compute_mean(const double *grades, int count);
double compute_min(const double *grades, int count);
double compute_max(const double *grades, int count);
double compute_median(const double *sorted_grades, int count);

void sort_grades(double *grades, int count);
void print_pointer_size(const double *grades);


int main(void)
{
    double grades[MAX_GRADES]; //creates array
    int count = 0;

    printf("How many grades would you like to enter? ");

    if (scanf("%d", &count) != 1)
    {
        printf("Invalid input.\n");
        return 1;
    }

    if (count <= 0 || count > MAX_GRADES)
    {
        printf("Invalid number of grades. Enter a value between 1 and %d.\n", MAX_GRADES);
        return 1; // checks if # of grades is valid
    }
    

    printf("Size of grades array: %zu\n", sizeof(grades)); //zu is for size_t or the format that sizeof returns

    //code that enters every grade into the array
    for (int i = 0; i < count; i++)
    {
        printf("Enter grade :%d (0-100): ", i + 1);

        if (scanf("%lf", &grades[i]) != 1)
        {
            printf("Invalid grade input. Use numbers.\n");
            return 1;
        }

        if (grades[i] < 0.0 || grades[i] > 100.0) //checks if the grades are within the correct range
        {
            printf("Invalid grade. (0-100)\n");
            return 1;
        }
    }

    //call to get array size inside of a function
    print_pointer_size(grades);

    double minimum = compute_min(grades, count);
    double maximum = compute_max(grades, count); //Math functions I am calling
    double mean = compute_mean(grades, count);

    sort_grades(grades, count); //sorts the grades prior to finding median

    double median = compute_median(grades, count);

    printf("\n         Grade Statistics         \n");
    printf("Number of grades: %d\n", count);
    printf("Lowest grade:     %.2f\n", minimum);
    printf("Highest grade:    %.2f\n", maximum); //prints all of the values
    printf("Mean:             %.2f\n", mean);
    printf("Median:           %.2f\n", median);

    return 0;
}

//finds the mean
double compute_mean(const double *grades, int count)
{
    double sum = 0.0;
    

    for (int i = 0; i < count; i++)
    {
        sum = sum + grades[i]; //adds every value in the array in a rolling total
    }

    return sum / count;
}


//finds the lowest grade
double compute_min(const double *grades, int count)
{
    double minimum = grades[0];

    for (int i = 1; i < count; i++)
    {
        if (grades[i] < minimum) //replaces the variable minimum with the next value in the array if the array value is smaller
        {
            minimum = grades[i];
        }
    }

    return minimum;
}


//finds the highest grade
double compute_max(const double *grades, int count)
{
    double maximum = grades[0];

    for (int i = 1; i < count; i++)
    {
        if (grades[i] > maximum)
        {
            maximum = grades[i]; //replaces the variable maximum with the next value in the array if the array value is bigger
        }
    }

    return maximum;
}


// uses selection sort to sort array
void sort_grades(double *grades, int count)
{
    for (int i = 0; i < count - 1; i++) //sets the position of the smallest value
    {
        int smallest = i;

        for (int j = i + 1; j < count; j++)
        {
            if (grades[j] < grades[smallest]) //finds the next smallest value till there isn't any
            {
                smallest = j;
            }
        }

        /* Swap the grades */
        double temporary = grades[i]; //temporary value to hold original number
        grades[i] = grades[smallest]; //moves smallest value to i's position
        grades[smallest] = temporary; //sets the previous value to where the original number was 
    }
}


//finds the median
double compute_median(const double *sorted_grades, int count)
{
    if (count % 2 == 1) //if the count is odd
    {
        return sorted_grades[count / 2];
    }
    else //the count is even
    {
        return (sorted_grades[count / 2 - 1] + sorted_grades[count / 2]) / 2.0;
        //takes the two middle numbers, adds them and divides them by 2 to find median
    }
}


//shows array decay
void print_pointer_size(const double *grades)
{
    /*
     * When an array is passed to a function, only is able to show the size of the pointer.
     which is why this will only print 8 bytes, for the size of the individual array.
     */
    printf("Size of grades parameter inside a function: %zu\n",
           sizeof(grades);
}


/* even test 1
How many grades would you like to enter? 6
Size of grades array: 800
Enter grade :1 (0-100): 76
Enter grade :2 (0-100): 92
Enter grade :3 (0-100): 25
Enter grade :4 (0-100): 12
Enter grade :5 (0-100): 35
Enter grade :6 (0-100): 54
Size of grades parameter inside a function: 8

         Grade Statistics         
Number of grades: 6
Lowest grade:     12.00
Highest grade:    92.00
Mean:             49.00
Median:           44.50

odd test 2

How many grades would you like to enter? 5
Size of grades array: 800
Enter grade :1 (0-100): 64
Enter grade :2 (0-100): 25
Enter grade :3 (0-100): 97
Enter grade :4 (0-100): 23
Enter grade :5 (0-100): 18
Size of grades parameter inside a function: 8

         Grade Statistics         
Number of grades: 5
Lowest grade:     18.00
Highest grade:    97.00
Mean:             45.40
Median:           25.00
*/
