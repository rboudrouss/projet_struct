#include <stdlib.h>
#include <time.h>
#include "tests.h"
#include "utils_tests.h"
#include <stdio.h>

int main()
{
    srand(time(NULL));
    print_f("this tests must be memory leaks free");

    keys_tests();
    prime_tests();
    protec_test();
    rsa_tests();
    signs_tests();

    return 0;
}
