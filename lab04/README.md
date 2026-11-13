
# Lab 04: Grade Statistics

Author: Ozzy Finnegan
Course: EET 3150 - C Programming

## Description

This program accepts student grades and calculates
basic statistics using arrays and functions in C.

The program calculates:
- Minimum grade
- Maximum grade
- Mean (average)
- Median

It also sorts the grades from lowest to highest
using a selection sort algorithm.

## Program Features

- Accepts between 1 and 100 grades.
- Validates that grades are between 0 and 100.
- Checks scanf() for invalid input.
- Stores grades in an array of doubles.
- Uses separate functions for each calculation.
- Uses const pointers for read-only functions.
- Demonstrates array-to-pointer conversion.
- Demonstrates sizeof() on an array and pointer.

## Functions

compute_mean()
    Calculates the average of all grades.

compute_min()
    Finds the lowest grade.

compute_max()
    Finds the highest grade.

sort_grades()
    Sorts grades in ascending order using
    selection sort.

compute_median()
    Finds the middle grade, or averages the
    two middle grades for an even count.

demonstrate_sizeof()
    Shows the difference between an array
    in main() and a pointer inside a function.

## Build and Run

Compile:
    make

Run:
    ./grade_statistics

Clean:
    make clean

Compiler flags:
    -Wall -Wextra -Werror

## Testing

Test 1: Odd Number of Grades
Input: 85, 92, 78, 95, 80

Minimum: 78.00
Maximum: 95.00
Mean: 86.00
Median: 85.00

Test 2: Even Number of Grades
Input: 90, 70, 80, 100

Minimum: 70.00
Maximum: 100.00
Mean: 85.00
Median: 85.00

Test 3: Boundary Values and Duplicates
Input: 0, 100, 75, 75, 50

Minimum: 0.00
Maximum: 100.00
Mean: 60.00
Median: 75.00

## Input Validation

The program was tested with invalid inputs.

Grade: 101
Result: Invalid grade entered.

Grade: abc
Result: Invalid grade entered.

Both invalid inputs were successfully rejected.

## Array and Pointer Demonstration

The grades array contains 100 doubles.

In main():
Array size: 800 bytes
Element size: 8 bytes
Array capacity: 100 elements

Inside demonstrate_sizeof():
Pointer size: 8 bytes

The array is converted to a pointer when passed
to a function. Therefore, the function receives
the element count as a separate parameter.
