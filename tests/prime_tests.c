#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "block.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// small primes
static int small_p[SM] = {2, 3, 5, 7, 41, 89, 139, 197, 239, 331, 599, 701, 829, 1103, 3797};
// small composits
static int small_np[SM] = {1, 4, 6, 8, 9, 15, 21, 51, 221, 343, 413, 777, 973, 1547, 4477};


void is_prime_naive_test()
{
    print_f("testing is_prime_naive function\n");
    for (int i = 0; i < SM; i++)
    {
        assert(is_prime_naive(small_p[i]));
        assert(!is_prime_naive(small_np[i]));
    }
}

void modpow_test()
{
    print_f("testing modpow function\n");
    assert(modpow(4271, 5, 15) == modpow_naive(4271, 5, 15));
    assert(modpow(331, 13, 22) == modpow_naive(331, 13, 22));
    assert(modpow(89, 23, 17) == modpow_naive(89, 23, 17));
    assert(modpow(3797, 22, 33) == modpow_naive(3797, 22, 33));
}

void rand_long_test()
{
    print_f("testing 'rand_long function'\n");
    for (int i = 0; i < NB_TEST; i++)
    {
        printf("randlong generated between %d and %d : %ld\n", 0, 100, rand_long(0, 100));
        printf("randlong generated between %d and %ld : %ld\n", 1000000, 10000000000, rand_long(1000000, 10000000000));
    }
}

void random_prime_number_test()
{
    print_f("testing 'random_prime_number' function'\n");
    for (int i = 0; i < NB_TEST; i++)
    {
        printf("random prime generated between %d and %d : %ld\n", 0, 100, random_prime_number(0, 100, 10));
        printf("random prime generated between %d and %ld : %ld\n", 1000000, 10000000000, random_prime_number(1000000, 1000000000, 10));
    }
}

void prime_tests()
{
    print_f("\nTesting 'prime.c' file\n\n");
    is_prime_naive_test();
    modpow_test();
    rand_long_test();
    random_prime_number_test();
}