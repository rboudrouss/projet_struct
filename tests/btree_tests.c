#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "btree.h"
#include "generate.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void create_node_tests()
{
    print_f("testing 'create_node_tests' and 'delete_node' functions\n");
    Block *b = generate_block(ARRAY_SIZE, "uwu", "uwu", 0);
    CellTree *t = create_node(b);
    assert(t && t->block && !t->father && t->height == 0 && !t->firstChild && !t->nextBro);
    ;
    assert(t->block == b);
    assert(!strcmp((char *)t->block->previous_hash, "uwu"));
    delete_node(t);
}

void add_child_tests()
{
    print_f("testing 'add_child', 'update_height', 'print_tree' and 'delete_tree' functions\n");
    CellTree *t = NULL;
    add_child(&t, create_node(generate_block(ARRAY_SIZE, "uwu", "1", 0)));
    add_child(&t, create_node(generate_block(ARRAY_SIZE, "uwu", "2", 0)));
    add_child(&t, create_node(generate_block(ARRAY_SIZE, "uwu", "2b", 0)));
    add_child(&t->firstChild, create_node(generate_block(ARRAY_SIZE, "uwu", "3", 0)));
    add_child(&t->firstChild->nextBro, create_node(generate_block(ARRAY_SIZE, "uwu", "4", 0)));

    assert(t && t->firstChild && !t->father && !t->nextBro);
    assert(t->firstChild->firstChild && t->firstChild->father == t && t->firstChild->nextBro && !strcmp((char *)t->firstChild->block->previous_hash, "2b"));
    assert(!t->firstChild->nextBro->nextBro);
    assert(t->firstChild->firstChild);
    assert(t->firstChild->nextBro->firstChild);
    assert(t->height == 2);

    print_tree(t);

    delete_tree(t);
}

void fusion_blocks_tests()
{
    print_f("testing 'highest_child' and 'fusion_blocks' functions\n");
    CellTree *t = generate_test_tree(ARRAY_SIZE, "uwu");
    CellProtected *p = fusion_blocks(t);
    print_list_protected(p);
    only_free_list_protected(p);
    delete_tree(t);
}

void create_block_tests()
{
    print_f("testing 'create_block' function\n");
    generate_random_pending(ARRAY_SIZE);
    Key *k = create_key(0, 0);
    create_block(NULL, k, 0);
    free_key(k);
    Block *b = read_block(PENDB);
    print_list_protected(b->votes);
    delete_block(b);
}

void add_block_tests()
{
    print_f("testing 'add_block' function\n");
    generate_random_pending(ARRAY_SIZE);
    Key *k = create_key(0, 0);
    create_block(NULL, k, 0);
    free_key(k);
    add_block(0, BCFOLDER_T "/t1.blk");

    Block *b = read_block(PENDB);
    print_list_protected(b->votes);
    delete_block(b);
}

void btree_tests()
{
    print_f("\ntesting 'btree.c' file\n\n");
    create_node_tests();
    add_child_tests();
    fusion_blocks_tests();
    create_block_tests();
    add_block_tests();
}