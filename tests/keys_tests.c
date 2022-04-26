#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "block.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_key_tests()
{
    print_f("testing 'init_key' and 'free_key' function\n");

    Key *k = malloc(sizeof(Key));
    init_key(k, 2, 4);
    assert(k->val == 2);
    assert(k->n == 4);

    free_key(k);
}

void key_cmp_tests()
{
    print_f("testing 'key_cmp' function\n");
    Key *k = malloc(sizeof(Key));
    Key *k2 = malloc(sizeof(Key));
    init_key(k, 1, 2);
    init_key(k2, 1, 2);
    assert(key_cmp(k, k2));
    init_key(k2, 2, 3);
    assert(!key_cmp(k, k2));
    free_key(k);
    free_key(k2);
}

void init_pair_keys_tests()
{
    print_f("testing 'init_pair_keys' function\n");
    Key *k = malloc(sizeof(Key));
    Key *k2 = malloc(sizeof(Key));
    print_f("testing 'init_pair_keys' function\n");
    init_pair_keys(k, k2, 100, 400);

    assert(k && k2);
    long *l = encrypt(TEST_MSG, k->val, k->n);
    char *str = decrypt(l, strlen(TEST_MSG), k2->val, k->n);
    assert(!strcmp(TEST_MSG, str));

    free(l);
    free(str);
    free_key(k);
    free_key(k2);
}

void key_to_str_tests()
{
    Key *k = malloc(sizeof(Key));
    init_key(k, 2, 3);
    char *str = key_to_str(k);
    assert(!strcmp("(2,3)", str));
    free(str);
    init_key(k, 1586432, 654198);
    str = key_to_str(k);
    assert(!strcmp("(183500,9fb76)", str));
    free(str);
    free_key(k);

    char *str2;
    k = malloc(sizeof(Key));
    Key *k2 = malloc(sizeof(Key));
    init_pair_keys(k, k2, 100, 300);
    str = key_to_str(k);
    str2 = key_to_str(k2);
    printf("Generated key pair : p = %s s = %s\n", str, str2);
    free(str);
    free(str2);
    free_key(k);
    free_key(k2);
}

void str_to_key_tests()
{
    char *str = "(12F3,4894)";
    Key *k = str_to_key(str);
    assert(k->n == 18580 && k->val == 4851);
    free_key(k);
}

void keys_tests()
{
    print_f("\ntesting 'keys.c' file\n\n");
    init_key_tests();
    key_cmp_tests();
    init_pair_keys_tests();
    key_to_str_tests();
    str_to_key_tests();
}