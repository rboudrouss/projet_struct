#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "block.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void init_protected_tests()
{
    print_f("testing 'init_protected' and 'free_protected' functions\n");
    Key *k = malloc(sizeof(Key));
    Key *k2 = malloc(sizeof(Key));
    init_pair_keys(k, k2, 100, 300);
    Signature *si = sign(TEST_MSG, k2);

    init_key(k, k->val, k->n);
    Protected *p = init_protected(k, TEST_MSG, si);
    free_protected(p);
    free_key(k2);
}

void verify_tests()
{
    print_f("testing 'verify' function\n");
    Key *k = malloc(sizeof(Key));
    Key *k2 = malloc(sizeof(Key));
    init_pair_keys(k, k2, 100, 300);
    Signature *si = sign(TEST_MSG, k2);
    Protected *p = init_protected(k, TEST_MSG, si);
    assert(verify(p));
    si->size--;
    assert(!verify(p));
    si->size++;
    si->content[0]++;
    assert(!verify(p));
    free_protected(p);
    free_key(k2);
}

void protec_test()
{
    print_f("\ntesting 'protec.c' file\n\n");
    verify_tests();
    init_protected_tests();
}