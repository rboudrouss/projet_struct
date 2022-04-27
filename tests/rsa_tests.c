#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "block.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// small primes
static int small_p[SM] = {2, 3, 5, 7, 41, 89, 139, 197, 239, 331, 599, 701, 829, 1103, 3797};
// big primes
static long big_p[B] = {2147483647, 2147482873, 2147480897, 2147478727, 2147470679, 2147469173, 2147465563, 2147463491, 2147462579, 2147462143};

void generate_key_value_tests()
{
    print_f("testing 'generate_key_value' function\n");
    long n, s, u;
    generate_key_values(big_p[0], big_p[1], &n, &s, &u);
    printf("key generated p=%ld q=%ld n=%ld s=%ld u=%ld\n", big_p[0], big_p[1], n, s, u);
    generate_key_values(small_p[SM - 1], small_p[SM - 2], &n, &s, &u);
    printf("key generated p=%d q=%d n=%ld s=%ld u=%ld\n", small_p[SM - 1], small_p[SM - 2], n, s, u);
}

void encrypt_tests()
{
    print_f("testing 'encrypt' function\n");
    long *l = encrypt("uwu", 1, 300);
    printf("must be equal to [%d %d %d] : ", 'u', 'w', 'u');
    print_long_vector(l, 3);
    free(l);
    l = encrypt("uwu", 524, 600);
    printf("must be equal to [321 121 321 ] : ");
    print_long_vector(l, 3);
    free(l);
}

void decrypt_tests()
{
    long n, s, u, *l;
    generate_key_values(small_p[SM - 1], small_p[SM - 2], &n, &s, &u);

    print_f("testing 'decrypt' function\n");
    if (u < 0)
    {
        long t = (small_p[SM - 1] - 1) * (small_p[SM - 2] - 1);
        u = u + t;
    }

    l = encrypt(TEST_MSG, s, n);
    char *str = decrypt(l, strlen(TEST_MSG), u, n);
    assert(!strcmp(TEST_MSG, str));
    free(str);
    free(l);
}

void rsa_tests()
{
    print_f("\ntesting 'rsa.c' file\n\n");
    generate_key_value_tests();
    encrypt_tests();
    decrypt_tests();
}