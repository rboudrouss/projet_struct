#include "tests.h"
#include "utils_tests.h"
#include "prime.h"
#include "rsa.h"
#include "block.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void write_block_tests()
{
    print_f("testing 'write_block' and 'delete_block' function\n");
    Block *b = generate_block(ARRAY_SIZE, "uwu", "uwu1", "uwu2");
    write_block(PENDB_T, b);

    FILE *f = fopen(PENDB_T, "r");
    char key_str[STR_SIZE], hash[STR_SIZE], prev_hash[STR_SIZE];
    int nonce;
    fscanf(f, "%s %s %s %d\n", key_str, hash, prev_hash, &nonce);

    char pr_str[STR_SIZE], temp1[STR_SIZE], temp2[STR_SIZE];
    fscanf(f, "%s %s %s\n", pr_str, temp1, temp2);

    fclose(f);

    assert(!strcmp(key_str, "(0,0)"));
    assert(!strcmp(hash, "uwu1"));
    assert(!strcmp(prev_hash, "uwu2"));
    assert(!strcmp(temp1, "uwu"));
    delete_block(b);

    write_block(PENDB_T, NULL);
}

void read_block_tests()
{
    print_f("testing 'read_block' function\n");
    Block *b = malloc(sizeof(Block));
    b->author = create_key(0,0);
    b->nonce = 0;
    b->hash = (unsigned char *)strdup("uwu1");
    b->previous_hash = (unsigned char *)strdup("uwu2");
    b->votes = create_cell_protected(init_protected(
        create_key(0, 1),
        "uwu",
        init_signature(NULL, 0)));
    write_block(PENDB_T, b);
    delete_block(b);

    b = read_block(PENDB_T);
    assert(b->author->n == 0 && b->author->val == 0);
    assert(!strcmp((char *)b->hash, "uwu1"));
    assert(!strcmp((char *)b->previous_hash, "uwu2"));
    assert(b->nonce == 0);
    assert(!b->votes->next);
    assert(b->votes->data->pKey->n == 1);
    assert(!strcmp(b->votes->data->mess, "uwu"));
    delete_block(b);
}

// TODO tester le reste mais je sais que ça marche

void block_tests()
{
    print_f("\ntesting 'block.c' file\n\n");
    write_block_tests();
    read_block_tests();
}