#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "generate.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void generate_tests()
{
    print_f("\ntesting 'generate.c' file\n\n");
    generate_random_data(ARRAY_SIZE, (int)ARRAY_SIZE / 10 + 2, (int)ARRAY_SIZE / 5 - 1);
    generate_random_pending(ARRAY_SIZE - 1);
}