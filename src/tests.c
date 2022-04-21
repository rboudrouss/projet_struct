#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "prime.h"
#include "rsa.h"
#include "block.h"

#define SM 15
#define B 10
#define NB_TEST 2
#define TEST_MSG "uwu"

#define RED "\033[31m"
#define WHITE "\033[97m"

#define print_f(a) printf(RED a WHITE)

void print_long_vector(long *result, int size)
{
    printf(" [ ");
    for (int i = 0; i < size; i++)
    {
        printf("%ld \t ", result[i]);
    }
    printf("] \n");
}

int main()
{
    srand(time(NULL));

    print_f("This test case must be memory-leak-free.\n");
    // small primes
    int small_p[SM] = {2, 3, 5, 7, 41, 89, 139, 197, 239, 331, 599, 701, 829, 1103, 3797};
    // small composits
    int small_np[SM] = {1, 4, 6, 8, 9, 15, 21, 51, 221, 343, 413, 777, 973, 1547, 4477};
    // big primes
    long big_p[B] = {2147483647, 2147482873, 2147480897, 2147478727, 2147470679, 2147469173, 2147465563, 2147463491, 2147462579, 2147462143};
    // big composits
    // long big_np[B] = {1074013595, 1074013577, 2147508417, 2147508345, 2147508277, 2147507647, 2147507361, 2147506863, 2147506447, 2147506153};

    print_f("\nTesting 'prime.c' file\n\n");

    print_f("testing is_prime_naive function\n");
    for (int i = 0; i < SM; i++)
    {
        assert(is_prime_naive(small_p[i]));
        assert(!is_prime_naive(small_np[i]));
    }
    print_f("testing modpow function\n");
    assert(modpow(4271, 5, 15) == modpow_naive(4271, 5, 15));
    assert(modpow(331, 13, 22) == modpow_naive(331, 13, 22));
    assert(modpow(89, 23, 17) == modpow_naive(89, 23, 17));
    assert(modpow(3797, 22, 33) == modpow_naive(3797, 22, 33));

    print_f("testing 'rand_long function'\n");
    for (int i = 0; i < NB_TEST; i++)
    {
        printf("randlong generated between %d and %d : %ld\n", 0, 100, rand_long(0, 100));
        printf("randlong generated between %d and %ld : %ld\n", 1000000, 10000000000, rand_long(1000000, 10000000000));
    }

    print_f("testing 'random_prime_number' function'\n");
    for (int i = 0; i < NB_TEST; i++)
    {
        printf("random prime generated between %d and %d : %ld\n", 0, 100, random_prime_number(0, 100, 10));
        printf("random prime generated between %d and %ld : %ld\n", 1000000, 10000000000, random_prime_number(1000000, 1000000000, 10));
    }

    print_f("\ntesting 'rsa.c' file\n\n");

    print_f("testing 'generate_key_value' function\n");

    long n, s, u;
    generate_key_values(big_p[0], big_p[1], &n, &s, &u);
    printf("key generated p=%ld q=%ld n=%ld s=%ld u=%ld\n", big_p[0], big_p[1], n, s, u);
    generate_key_values(small_p[SM - 1], small_p[SM - 2], &n, &s, &u);
    printf("key generated p=%d q=%d n=%ld s=%ld u=%ld\n", small_p[SM - 1], small_p[SM - 2], n, s, u);

    print_f("testing 'encrypt' function\n");

    long *l = encrypt("uwu", 1, 300);
    printf("must be equal to [%d %d %d] : ", 'u', 'w', 'u');
    print_long_vector(l, 3);
    free(l);
    l = encrypt("uwu", 524, 600);
    printf("must be equal to [321 121 321 ] : ");
    print_long_vector(l, 3);
    free(l);

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

    print_f("\ntesting 'keys.c' file\n\n");

    Key *k = malloc(sizeof(Key)), *k2 = NULL;
    print_f("testing 'init_key' function\n");

    init_key(k, 2, 4);
    assert(k->val == 2);
    assert(k->n == 4);

    print_f("testing 'free_key' function\n");
    free_key(k);

    print_f("testing 'key_cmp' function\n");
    k = malloc(sizeof(Key));
    k2 = malloc(sizeof(Key));
    init_key(k, 1, 2);
    init_key(k2, 1, 2);
    assert(key_cmp(k, k2));
    init_key(k2, 2, 3);
    assert(!key_cmp(k, k2));

    print_f("testing 'init_pair_keys' function\n");
    init_pair_keys(k, k2, 100, 400);

    assert(k && k2);

    l = encrypt(TEST_MSG, k->val, k->n);
    str = decrypt(l, strlen(TEST_MSG), k2->val, k->n);
    assert(!strcmp(TEST_MSG, str));

    free(l);
    free(str);
    free_key(k);
    free_key(k2);

    print_f("testing 'key_to_str' function\n");

    k = malloc(sizeof(Key));
    init_key(k, 2, 3);
    str = key_to_str(k);
    assert(!strcmp("(2,3)", str));
    free(str);
    init_key(k, 1586432, 654198);
    str = key_to_str(k);
    assert(!strcmp("(183500,9fb76)", str));
    free(str);
    free_key(k);

    char *str2;
    k = malloc(sizeof(Key));
    k2 = malloc(sizeof(Key));
    init_pair_keys(k, k2, 100, 300);
    str = key_to_str(k);
    str2 = key_to_str(k2);
    printf("Generated key pair : p = %s s = %s\n", str, str2);
    free(str);
    free(str2);
    free_key(k);
    free_key(k2);

    print_f("testing 'str_to_key' function\n");
    str = "(12F3,4894)";
    k = str_to_key(str);
    assert(k->n == 18580 && k->val == 4851);
    free_key(k);

    print_f("\ntesting 'signs.c' file\n\n");

    print_f("testing 'init_signature' and 'free_signature' functions\n");
    Signature *si = init_signature(NULL, 0);
    assert(si->content == NULL && si->size == 0);
    free_signature(si);
    long li[3] = {15623, 58489, 35168};
    si = init_signature(li, 3);
    assert(si->content[2] && si->size == 3);
    free_signature(si);

    print_f("testing 'sign' function\n");
    k = malloc(sizeof(Key));
    k2 = malloc(sizeof(Key));
    init_pair_keys(k, k2, 100, 300);
    si = sign(TEST_MSG, k2);
    str = decrypt(si->content, si->size, k->val, k->n);
    assert(!strcmp(str, TEST_MSG));
    free(str);

    print_f("\ntesting 'protec.c' file\n\n");

    print_f("testing 'init_protected' and 'free_protected' functions\n");
    Key *k3 = malloc(sizeof(Key));
    Signature *si2 = init_signature(si->content, si->size);
    init_key(k3, k->val, k->n);
    Protected *p = init_protected(k3, TEST_MSG, si2);
    free_protected(p);

    print_f("testing 'verify' function\n");
    k3 = malloc(sizeof(Key));
    si2 = init_signature(si->content, si->size);
    init_key(k3, k->val, k->n);
    p = init_protected(k3, TEST_MSG, si2);
    assert(verify(p));
    si2->size--;
    assert(!verify(p));
    si2->size++;
    si2->content[0]++;
    assert(!verify(p));
    free_protected(p);

    // TODO protected_to_str, et l'autre
    print_f("\ntesting 'cells.c' file\n\n");

    free_key(k);
    free_key(k2);
    free_signature(si);

    return 0;
}
