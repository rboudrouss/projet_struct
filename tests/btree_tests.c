#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "btree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void create_node_tests()
{
    print_f("testing 'create_node_tests' function\n");
    Block *b = generate_block(ARRAY_SIZE, "uwu", "uwu", "uwu");
    CellTree *t = create_node(b);
    assert(t && t->block && !t->father && t->height == 0 && !t->firstChild && !t->nextBro);
    ;
    assert(t->block == b);
    assert(!strcmp((char*)t->block->hash, "uwu"));
    delete_node(t);
}

void bree_tests()
{
    create_node_tests();
}