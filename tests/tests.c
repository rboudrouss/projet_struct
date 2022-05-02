#include <stdlib.h>
#include <time.h>
#include "tests.h"
#include "utils_tests.h"
#include "generate.h"
#include <stdio.h>

int main()
{
    srand(time(NULL));
    print_f("this tests must be memory leaks free");

    prime_tests();
    rsa_tests();
    keys_tests();
    signs_tests();
    protec_test();
    cells_tests();
    generate_tests();
    block_tests();

    generate_random_data((int)ARRAY_SIZE * 1.5, 10, ARRAY_SIZE);

    btree_tests();

    return 0;
}
