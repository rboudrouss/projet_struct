#include "keys.h"
#include "protec.h"

#ifndef __CELLS__
#define __CELLS__

// liste chainé key
typedef struct cellKey
{
    Key* data;
    struct cellKey* next;
} CellKey;

// liste chainé signature
typedef struct cellProtected
{
    Protected* data;
    struct cellProtected* next;
} CellProtected;

// hash table
typedef struct hashcell
{
    Key* key;
    int val;
} HashCell;

typedef struct hastable
{
    HashCell** tab;
    int size;
} HashTable;


#endif