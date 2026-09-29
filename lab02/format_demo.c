/*
 * Author:      Merl Creps
 * Exercise:    Lab 2 companion sample - printf/scanf format specifiers
 * Description: Shows how to read int, double, char, and string input with
 *              scanf (checking its return value every time) and how to
 *              print each type with printf using width, precision, and
 *              alignment. Also shows integer vs. floating-point division.
 *
 * Build:  gcc -Wall -Wextra -Werror -o format_demo format_demo.c
 * Run:    ./format_demo
 */

#include <stdio.h>

/* Throw away the rest of the current input line (e.g. "abc" after a
 * failed %d). Without this, the bad characters stay in the buffer and
 * the next scanf fails on them again - an infinite loop. */
static void clear_line(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard */
    }
}

int main(void)
{
    int count;
    double weight;
    char grade;
    char name[32];

    /* ---------------------------------------------------------------
     * 1. Reading an int with %d - and checking scanf's return value.
     *    scanf returns how many items it matched: 1 here on success,
     *    0 if the user typed letters, EOF if input ended.
     *    The & in &count means "the address of count". scanf needs the
     *    address so it can store the value directly into the variable.
     *    printf does not use & because it only needs to read the value.
     * --------------------------------------------------------------- */
    printf("Enter a whole number: ");
    while (scanf("%d", &count) != 1) {
        /* scanf failed. There are two possible reasons:
         *   a) The user typed something that isn't a number ("abc").
         *      -> Clear the bad line and ask again.
         *   b) There is no more input at all (the user pressed Ctrl+D,
         *      or input was piped from a file that ran out).
         *      -> Asking again would loop forever, so stop.
         * feof(stdin) ("end of file" on standard input) is true only in
         * case b. Note: feof becomes true AFTER a read fails, which is
         * why we check scanf's return value first and only use feof to
         * find out WHY it failed. Do not write while (!feof(stdin)). */
        if (feof(stdin)) {
            printf("\nNo input. Exiting.\n");
            return 1;
        }
        printf("  Not a whole number. Try again: ");
        clear_line();
    }

    /* ---------------------------------------------------------------
     * 2. Reading a double: scanf needs %lf.
     *
     *    The "l" is a LENGTH MODIFIER meaning "long". %lf is informally
     *    "long float", which in C is a double.
     *        %f  -> float        (4 bytes)
     *        %lf -> double       (8 bytes)
     *        %Lf -> long double  (capital L)
     *
     *    Why scanf cares: scanf gets a POINTER (&weight) to where it
     *    should store the number, so it must know how many bytes to
     *    write. Using %f with a double writes only 4 of the 8 bytes
     *    and leaves you with garbage.
     *
     *    Why printf doesn't: printf gets the VALUE, and C automatically
     *    promotes a float argument to double. So printf always receives
     *    a double, and %f works for both float and double.
     *
     *    Rule: double -> %lf in scanf, %f in printf.
     * --------------------------------------------------------------- */
    printf("Enter a weight in pounds (e.g. 12.5): ");
    while (scanf("%lf", &weight) != 1) {
        /* Same pattern as step 1: stop if input ran out, otherwise
         * clear the bad line and ask again. */
        if (feof(stdin)) {
            printf("\nNo input. Exiting.\n");
            return 1;
        }
        printf("  Not a number. Try again: ");
        clear_line();
    }

    /* ---------------------------------------------------------------
     * 3. Reading a char: the space before %c skips the leftover newline
     *    from the previous Enter key. Without it, %c reads '\n'.
     * --------------------------------------------------------------- */
    printf("Enter a letter grade (A-F): ");
    if (scanf(" %c", &grade) != 1) {
        printf("\nNo input. Exiting.\n");
        return 1;
    }

    /* ---------------------------------------------------------------
     * 4. Reading a word: %31s limits input to 31 chars + '\0' so a long
     *    name cannot overflow the 32-byte array. No & for arrays.
     * --------------------------------------------------------------- */
    printf("Enter your first name: ");
    if (scanf("%31s", name) != 1) {
        printf("\nNo input. Exiting.\n");
        return 1;
    }

    /* ---------------------------------------------------------------
     * 5. printf format specifiers
     * --------------------------------------------------------------- */
    printf("\n--- int ---\n");
    printf("%%d      -> [%d]\n", count);         /* plain                 */
    printf("%%6d     -> [%6d]\n", count);        /* width 6, right-aligned*/
    printf("%%-6d    -> [%-6d]\n", count);       /* width 6, left-aligned */
    printf("%%06d    -> [%06d]\n", count);       /* zero-padded           */
    printf("%%+d     -> [%+d]\n", count);        /* always show sign      */
    printf("%%x / %%o -> [%x] / [%o]\n", (unsigned)count, (unsigned)count);
                                                 /* hex / octal           */

    printf("\n--- double ---\n");
    printf("%%f      -> [%f]\n", weight);        /* default 6 decimals    */
    printf("%%.2f    -> [%.2f]\n", weight);      /* 2 decimals            */
    printf("%%10.3f  -> [%10.3f]\n", weight);    /* width 10, 3 decimals  */
    printf("%%-10.1f -> [%-10.1f]\n", weight);   /* left-aligned          */
    printf("%%e      -> [%e]\n", weight);        /* scientific notation   */
    printf("%%g      -> [%g]\n", weight);        /* shortest of %f / %e   */

    printf("\n--- char and string ---\n");
    printf("%%c      -> [%c]\n", grade);
    printf("%%d on a char -> [%d]  (its ASCII code)\n", grade);
    printf("%%s      -> [%s]\n", name);
    printf("%%10s    -> [%10s]\n", name);        /* right-aligned in 10   */
    printf("%%-10s   -> [%-10s]\n", name);       /* left-aligned in 10    */

    /* ---------------------------------------------------------------
     * 6. A formatted table - width and precision make columns line up.
     * --------------------------------------------------------------- */
    printf("\n%-10s %8s %10s\n", "Name", "Grade", "Weight kg");
    printf("%-10s %8c %10.2f\n", name, grade, weight * 0.45359237);

    /* ---------------------------------------------------------------
     * 7. Integer vs. floating-point division.
     *    7 / 2 : both operands are int, so C does integer division and
     *            truncates toward zero -> 3.
     *    7.0 / 2 or (double)7 / 2 : one operand is double, so the int is
     *            converted and C does floating-point division -> 3.5.
     * --------------------------------------------------------------- */
    printf("\n--- division ---\n");
    printf("7 / 2         = %d\n", 7 / 2);
    printf("7.0 / 2       = %.1f\n", 7.0 / 2);
    printf("(double)7 / 2 = %.1f\n", (double)7 / 2);
    printf("%d / 2 as int = %d, as double = %.2f\n",
           count, count / 2, count / 2.0);

    return 0;
}

/*
 * Sample run (valid and invalid input):
 *
 *   Enter a whole number: abc
 *     Not a whole number. Try again: 42
 *   Enter a weight in pounds (e.g. 12.5): ten
 *     Not a number. Try again: 12.5
 *   Enter a letter grade (A-F): B
 *   Enter your first name: Merl
 *   ...
 */
