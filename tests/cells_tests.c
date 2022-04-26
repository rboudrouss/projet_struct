#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "block.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void create_cell_key_tests()
{
    print_f("testing 'create_cell_key', 'delete_cell_key' and 'delete_list_keys' functions \n");
    Key *k = malloc(sizeof(Key));
    init_key(k, 10, 2);
    CellKey *l = create_cell_key(k);
    assert(l->data == k);
    assert(l->data->n == 2 && l->data->val == 10);
    delete_cell_key(l);
}

// TODO test read_public_keys/_fromp print_list_keys

void add_cell_key_tests()
{
    print_f("testing 'add_cell_key', 'delete_list_key' and 'delete_list_keys' functions \n");
    Key *k = malloc(sizeof(Key)), *k2=malloc(sizeof(Key));
    init_key(k, 10, 2);
    init_key(k2, 10, 2);
    CellKey *l = create_cell_key(k), *l2;
    l2 = l;
    add_cell_key(&l, k2);

    assert(l->data == k2);
    assert(l->next->data == k);
    assert(l->next == l2);

    delete_list_keys(l);
}

void cells_tests()
{
    create_cell_key_tests();
    add_cell_key_tests();
}