#include <stdio.h>
int main(void) {
    int n;
    printf("Enter a number: ");
    if (scanf("%d", &n) != 1) {
        return 1; 
    }
    if (n < 1) {
        printf("Invalid\n");
        return 0;
    }
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        if (i % 2 == 0) {
            printf("%d even\n", i);
        } else {
            printf("%d odd\n", i);
        }
        sum += i;
    }
    double average = (double)sum / n;
    
    printf("Sum: %d\n", sum);
    printf("Average: %.2f\n", average);
    return 0;
}
