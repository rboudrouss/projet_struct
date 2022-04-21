#include "block.h"

#ifndef __BTREE__
#define __BTREE__

typedef struct block_tree_cell
{
    Block *block;
    struct block_tree_cell *father;
    struct block_tree_cell *firstChild;
    struct block_tree_cell *nextBro;
    int height;
} CellTree;

CellTree *init_father_tree(CellProtected *c, int d);
CellTree *create_node(Block *b);
int update_height(CellTree *father, CellTree *child);
void add_child(CellTree **father, CellTree *child);
void print_tree(CellTree *node);
void delete_node(CellTree *node);
void delete_tree(CellTree *tree);
CellTree *highest_child(CellTree *cell);
CellTree *last_node(CellTree *tree);
CellProtected *fusion_blocks(CellTree *tree);
void create_block(CellTree *tree, Key *author, int d);
void add_block(int d, char *name);
CellTree *read_tree();
#endif