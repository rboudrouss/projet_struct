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
    Block *b = generate_block(ARRAY_SIZE, "uwu1", "uwu2", 0);
    write_block(PENDB_T, b);

    FILE *f = fopen(PENDB_T, "r");
    char key_str[STR_SIZE], hash[STR_SIZE], prev_hash[STR_SIZE];
    int nonce;
    fscanf(f, "%s %s %s %d\n", key_str, hash, prev_hash, &nonce);

    char pr_str[STR_SIZE], temp1[STR_SIZE], temp2[STR_SIZE];
    fscanf(f, "%s %s %s\n", pr_str, temp1, temp2);

    fclose(f);

    printf("%s %s\n", b->previous_hash, prev_hash);
    assert(!strcmp(key_str, "(0,0)"));
    assert(!strcmp(prev_hash, "uwu2"));
    assert(!strcmp(temp1, "uwu1"));
    delete_block(b);

    write_block(PENDB_T, NULL);
}

void read_block_tests()
{
    print_f("testing 'read_block' function\n");
    Block *b = malloc(sizeof(Block));
    b->author = create_key(0, 0);
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

    b = generate_block(ARRAY_SIZE, "uwu", NULL, NBD_T);
    char *str1 = block_to_str(b);
    printf("str before RB : %s\n", str1);
    write_block(PENDB_T, b);
    delete_block(b);

    b = read_block(PENDB_T);
    char *str2 = block_to_str(b);
    printf("str after RB : %s\n", str2);
    assert(!strcmp(str1, str2));
    delete_block(b);
    free(str1);
    free(str2);
}

void block_to_str_tests()
{
    print_f("testing 'block_to_str' function\n");
    Block *b = generate_block(ARRAY_SIZE, "uwu", NULL, 0);
    char *s = block_to_str(b);
    printf("generated block_to_str : %s\n", s);
    printf("from list : \n");
    print_list_protected(b->votes);
    free(s);
    delete_block(b);
}

void hashf_tests()
{
    print_f("testing 'hashf' function\n");
    unsigned char *hash;
    hash = hashf(TEST_MSG);
    printf("hashed : %s\n", hash);
    free(hash);
}

void compute_proof_of_work_tests()
{
    print_f("testing 'compute_proof_of_work' function\n");
    Block *b = generate_block(ARRAY_SIZE, "uwu", NULL, NBD_T);
    printf("final nonce %d with hash %s needing %d zeros\n", b->nonce, b->hash, NBD_T);
    delete_block(b);
}

void verify_block_tests()
{
    print_f("testing 'verify_block' function\n");
    Block *b = generate_block(ARRAY_SIZE, "uwu", NULL, NBD_T);
    assert(verify_block(b, NBD_T));
    delete_block(b);
    b = generate_block(ARRAY_SIZE, "uwu", NULL, NBD_T);
    free(b->hash);
    b->hash = (unsigned char *)strdup("non valid hash");
    assert(!verify_block(b, NBD_T));
    delete_block(b);
}

void block_tests()
{
    print_f("\ntesting 'block.c' file\n\n");
    write_block_tests();
    read_block_tests();
    hashf_tests();
    block_to_str_tests();
    compute_proof_of_work_tests();
    verify_block_tests();
}