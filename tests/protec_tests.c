#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "block.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_protected_tests()
{
    print_f("testing 'init_protected' and 'free_protected' functions\n");
    Key *k = malloc(sizeof(Key));
    init_key(k, 0, 0);
    Signature *si = init_signature(NULL, 0);
    Protected *p = init_protected(k, TEST_MSG, si);

    assert(!strcmp(TEST_MSG, p->mess));
    assert(p->sgn == si);
    assert(p->pKey == k);
    assert(p->pKey->val == 0);
    assert(p->sgn->content == NULL);
    free_protected(p);
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

void protected_to_str_tests()
{
    print_f("testing 'protected_to_str' function\n");
    Key *k = malloc(sizeof(Key));
    init_key(k, 1, 12);
    long l[3] = {16, 100, '\0'};
    Signature *si = init_signature(l, 3);
    Protected *p = init_protected(k, "uwu", si);
    char* s = protected_to_str(p);
    assert(!strcmp("(1,c) uwu #10#64#0#", s));
    free(s);
    free_protected(p);
}

void str_to_protected_tests()
{
    Protected *p = str_to_protected("(1,c) uwu #10#64#0#");
    long l[3] = {16, 100, '\0'};
    assert(p->pKey->val == 1 && p->pKey->n == 12);
    assert(p->sgn->size == 3 && longl_cmp(l,p->sgn->content));
    assert(!strcmp("uwu", p->mess));
    free_protected(p);
}

void protec_test()
{
    print_f("\ntesting 'protec.c' file\n\n");
    verify_tests();
    init_protected_tests();
    protected_to_str_tests();
}
