#include "protec.h"

#ifndef __CELLS__
#define __CELLS__

// liste chainé key
typedef struct cellKey
{
    Key *data;
    struct cellKey *next;
} CellKey;

// liste chainé signature
typedef struct cellProtected
{
    Protected *data;
    struct cellProtected *next;
} CellProtected;

// hash table
typedef struct hashcell
{
    Key *key;
    int val;
} HashCell;

typedef struct hastable
{
    HashCell **tab;
    int size;
} HashTable;

// CellKey
CellKey *create_cell_key(Key *key);
CellKey *read_public_keys(char *name);
void add_cell_key(CellKey **l, Key *k);
void print_list_keys(CellKey *LCK);
void delete_cell_key(CellKey *c);
void delete_list_keys(CellKey *c);

// CellProtected
CellProtected *create_cell_protected(Protected *pr);
void add_cell_protected(CellProtected **c, Protected *p);
CellProtected *read_protected(char *name);
void print_list_protected(CellProtected *l);
void delete_cell_protected(CellProtected *c);
void delete_list_protected(CellProtected *c);
void delete_non_valid(CellProtected **c);

// HashTable
HashCell *create_hashcell(Key *key);
int hash_function(Key *key, int size);
int find_position(HashTable *t, Key *key);
void insert_key_table(HashTable *t, Key *key);
HashCell *get_cell_table(HashTable *t, Key *key);
HashTable *create_hashtable(CellKey *keys, int size);
void delete_hashtable(HashTable *t);
Key *compute_winner(CellProtected *decl, CellKey *candidates, CellKey *voters, int sizeC, int sizeV);

#endif