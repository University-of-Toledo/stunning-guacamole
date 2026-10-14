#include <stdio.h>

int main(void)
{
    int n;
    int sum = 0;
    double average;

    printf("Enter a number: ");
    scanf("%d", &n);

    if (n < 1)
    {
        printf("Invalid\n");
        return 0;
    }

    for (int i = 1; i <= n; i++)
    {
        if (i % 2 == 0)
        {
            printf("%d even\n", i);
        }
        else
        {
            printf("%d odd\n", i);
        }

        sum = sum + i;
    }

    average = (double)sum / n;

    printf("Sum: %d\n", sum);
    printf("Average: %.2f\n", average);

    return 0;
}
