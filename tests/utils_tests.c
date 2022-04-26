#include "utils_tests.h"
#include <stdio.h>

void print_long_vector(long *result, int size)
{
    printf(" [ ");
    for (int i = 0; i < size; i++)
    {
        printf("%ld \t ", result[i]);
    }
    printf("] \n");
}