#include "cells.h"

#ifndef __BLOCK__
#define __BLOCK__

typedef struct block
{
    Key *author;
    CellProtected *votes;
    unsigned char *hash;
    unsigned char *previous_hash;
    int nonce;
} Block;

void write_block(char *filename, Block *block);
Block *read_block(char *name);
char *block_to_str(Block *block);
unsigned char *hashf(char *s, int nonce);
int first_d_zero(unsigned char *s, int d);
void compute_proof_of_work(Block *b, int d);
int verify_block(Block *b, int d);
void delete_block(Block *b);

#endif