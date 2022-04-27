#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "block.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// CellKey
void create_cell_key_tests()
{
    print_f("testing 'create_cell_key', 'delete_cell_key' and 'delete_list_keys' functions \n");
    Key *k = create_key(10, 2);
    CellKey *l = create_cell_key(k);
    assert(l->data == k);
    assert(l->data->n == 2 && l->data->val == 10);
    assert(!l->next);
    delete_cell_key(l);
}

void read_public_keys_tests()
{
    print_f("testing 'read_public_keys' function\n");
    FILE *f = fopen(CANDF_T, "w");
    fprintf(f, "(12,4)\n(153,1234)\n(496,189)\n");
    fclose(f);

    CellKey *l = read_public_keys(CANDF_T);
    assert(l);
    assert(l->data && l->data->val == 1174 && l->data->n == 393);
    assert(l->next && l->next->data && l->next->data->val == 339 && l->next->data->n == 4660);
    assert(l->next->next && l->next->next->data && l->next->next->data->val == 18 && l->next->next->data->n == 4);
    assert(!l->next->next->next);

    delete_list_keys(l);
}

void read_public_keys_fromp_tests()
{
    print_f("testing 'read_public_keys_fromp' function\n");
    FILE *f = fopen(KEYSF_T, "w");
    fprintf(f, "(12,4) (16,5)\n(153,1234) (758,394)\n(496,189) (8646,553)\n");
    fclose(f);

    CellKey *l = read_public_keys(KEYSF_T);
    assert(l);
    assert(l->data && l->data->val == 1174 && l->data->n == 393);
    assert(l->next && l->next->data && l->next->data->val == 339 && l->next->data->n == 4660);
    assert(l->next->next && l->next->next->data && l->next->next->data->val == 18 && l->next->next->data->n == 4);
    assert(!l->next->next->next);

    delete_list_keys(l);
}

void add_cell_key_tests()
{
    print_f("testing 'add_cell_key', 'delete_list_key' and 'delete_list_keys' functions \n");
    Key *k = create_key(10, 2), *k2 = create_key(10, 2);
    CellKey *l = create_cell_key(k), *l2;
    l2 = l;
    add_cell_key(&l, k2);

    assert(l->data == k2);
    assert(l->next->data == k);
    assert(l->next == l2);

    delete_list_keys(l);
}

void print_list_keys_tests()
{
    print_f("testing 'print_list_keys' function\n");
    CellKey *l = generate_cellkeys(ARRAY_SIZE);
    print_list_keys(l);
    delete_list_keys(l);
}

// CellProtected
void create_cell_protected_tests()
{
    print_f("testing 'create_cell_protected' and 'delete_cell_protected' function\n");
    Protected *p = init_protected(
        create_key(10, 4),
        TEST_MSG,
        init_signature(NULL, 0));
    CellProtected *l = create_cell_protected(p);
    assert(l && l->data && !l->next);
    assert(l->data == p);
    assert(l->data->pKey->val == 10 && l->data->pKey->n == 4);
    assert(!strcmp(l->data->mess, TEST_MSG));
    assert(l->data->sgn->content == NULL && l->data->sgn->size == 0);
    delete_cell_protected(l);
}

void add_cell_protected_tests()
{
    print_f("testing 'add_cell_protected' and 'delete_list_protected' functions\n");
    CellProtected *l = NULL;
    add_cell_protected(
        &l,
        init_protected(
            create_key(12, 4),
            TEST_MSG,
            init_signature(NULL, 0)));
    add_cell_protected(
        &l,
        init_protected(
            create_key(10, 2),
            TEST_MSG,
            init_signature(NULL, 0)));
    assert(l && l->data && l->next && !l->next->next);

    assert(l->data->pKey->val == 10 && l->data->pKey->n == 2);
    assert(!strcmp(l->data->mess, TEST_MSG));
    assert(l->data->sgn->content == NULL && l->data->sgn->size == 0);

    assert(l->next->data->pKey->val == 12 && l->next->data->pKey->n == 4);
    assert(!strcmp(l->next->data->mess, TEST_MSG));
    assert(l->next->data->sgn->content == NULL && l->next->data->sgn->size == 0);

    delete_list_protected(l);
}

void print_list_protected_tests()
{
    print_f("testing 'print_list_protected' function\n");
    CellProtected *l = generate_cellprotected(ARRAY_SIZE, TEST_MSG);
    add_cell_protected(&l, NULL);
    print_list_protected(l);
    delete_list_protected(l);
}

void fusion_protected_tests()
{
    print_f("Testing 'fusion_protected' function\n");
    CellProtected *p = generate_cellprotected(ARRAY_SIZE, TEST_MSG);
    CellProtected *p2 = generate_cellprotected(ARRAY_SIZE, TEST_MSG);
    CellProtected *p3 = p;
    for (; p3->next; p3 = p3->next)
        ;
    fusion_protected(p, p2);
    assert(p3->next == p2);
    delete_list_protected(p);
}

void delete_non_valid_tests()
{
    print_f("testing 'delete_non_valid' function\n");
    CellProtected *p = generate_cellprotected(ARRAY_SIZE, TEST_MSG);
    // no leaks
    delete_non_valid(&p);
    delete_list_protected(p);

    Key* k = malloc(sizeof(Key)), *k2 = malloc(sizeof(Key));
    init_pair_keys(k,k2,100,300);
    Signature* vs = sign(TEST_MSG, k2);
    free_key(k2);

    p = NULL;
    // non valid
    add_cell_protected(&p, init_protected(
        dup_key(k),
        TEST_MSG,
        init_signature(NULL,0)
    ));
    // valid
    add_cell_protected(&p, init_protected(
        dup_key(k),
        TEST_MSG,
        dup_sign(vs)
    ));
    // non valid
    k->val++;
    add_cell_protected(&p, init_protected(
        dup_key(k),
        TEST_MSG,
        dup_sign(vs)
    ));
    k->val--;
    // non valid
    vs->size--;
    add_cell_protected(&p, init_protected(
        dup_key(k),
        TEST_MSG,
        dup_sign(vs)
    ));
    vs->size++;
    // non valid
    vs->content[0]++;
    add_cell_protected(&p, init_protected(
        dup_key(k),
        TEST_MSG,
        dup_sign(vs)
    ));
    vs->content[0]--;

    delete_non_valid(&p);
    print_list_protected(p);
    assert(p && !p->next);
    assert(key_cmp(k,p->data->pKey));
    assert(!strcmp(p->data->mess,TEST_MSG));
    assert(vs->size == p->data->sgn->size);

    free_key(k);
    free_signature(vs);
}

void cells_tests()
{
    print_f("\ntesting 'cells.c' file\n\n");
    // CellKey
    create_cell_key_tests();
    add_cell_key_tests();
    print_list_keys_tests();
    read_public_keys_tests();
    read_public_keys_fromp_tests();
    
    // CellProtected
    create_cell_protected_tests();
    add_cell_protected_tests();
    print_list_protected_tests();
    fusion_protected_tests();
    delete_non_valid_tests();
}