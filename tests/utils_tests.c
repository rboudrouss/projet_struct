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

// they must finish with \0, 1 if == else 0
// <!> doesn't work if 0 in array <!>
int longl_cmp(long* l1, long* l2)
{
    for(;*l1 && *l2 && *l1==*l2;l1++, l2++);
    return *l1 == '\0' && *l2 == '\0';
}