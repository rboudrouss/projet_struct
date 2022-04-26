#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "block.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_signature_tests()
{
    print_f("testing 'init_signature' and 'free_signature' functions\n");
    Signature *si = init_signature(NULL, 0);
    assert(si->content == NULL && si->size == 0);
    free_signature(si);
    long li[3] = {15623, 58489, 35168};
    si = init_signature(li, 3);
    assert(si->content[2] && si->size == 3);
    free_signature(si);
}

void sign_tests()
{
    print_f("testing 'sign' function\n");
    Key *k = malloc(sizeof(Key));
    Key *k2 = malloc(sizeof(Key));
    init_pair_keys(k, k2, 100, 300);
    Signature *si = sign(TEST_MSG, k2);
    char *str = decrypt(si->content, si->size, k->val, k->n);
    assert(!strcmp(str, TEST_MSG));
    free_signature(si);
    free(str);
    free_key(k);
    free_key(k2);
}

void signs_tests()
{
    print_f("\ntesting 'signs.c' file\n\n");
    init_signature_tests();
    sign_tests();
}