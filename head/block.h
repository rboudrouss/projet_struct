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

#endif