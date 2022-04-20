#include <stdlib.h>
#include <stdio.h>
#include "btree.h"
#include "files.h"

CellTree *create_node(Block *b)
{
    CellTree *rep = malloc(sizeof(CellTree));
    rep->block = b;
    rep->father = rep->firstChild = rep->nextBro = NULL;
    rep->height = 0;
    return rep;
}

int update_height(CellTree *father, CellTree *child)
{
    if (father->height > (child->height + 1))
        return 0;
    father->height = child->height + 1;
    return 1;
}

void add_child(CellTree *father, CellTree *child)
{
    father->firstChild = child;

    for (; father->father; father = father->father)
        update_height(father->father, father);
}

void print_tree(CellTree *node)
{
    CellTree *c;
    for (; node; node = node->firstChild)
        for (c = node; c; c = c->nextBro)
        {
            printf("%s %d; ", node->block->hash, node->height);
            printf("\n");
        }
}

void delete_node(CellTree *node)
{
    if (!node)
        return;
    delete_block(node->block);
}

void delete_tree(CellTree *tree)
{
    if (!tree)
        return;
    delete_tree(tree->firstChild);
    delete_tree(tree->nextBro);

    delete_block(tree->block);
}

CellTree *highest_child(CellTree *cell)
{
    CellTree *highest, *current = cell;
    int max = -1;
    for (; current; current = current->nextBro)
        if (current->height > max)
        {
            highest = current;
            max = current->height;
        }
    return highest;
}

CellTree *last_node(CellTree *tree)
{
    CellTree *cell = highest_child(tree);
    if (!(cell && cell->firstChild))
        return cell;
    for (; cell->firstChild; cell = cell->firstChild)
        ;
    return cell;
}

CellProtected *fusion_blocks(CellTree *tree)
{
    CellTree *cell = highest_child(tree);
    CellProtected *rep = NULL;
    for (; cell; cell = cell->firstChild)
        fusion_protected(rep, cell->block->votes);
    return rep;
}

void create_block(CellTree *tree, Key *author, int d)
{
    CellProtected *pl = read_protected(PENDV);
    Block* b = malloc(sizeof(Block));
    b->author = author;
    b->previous_hash = last_node(tree)->block->hash;
    compute_proof_of_work(b,d);
    FILE* f = fopen(PENDB, "w");
    char* str = block_to_str(b);
    fputs(str,f);
    fclose(f);
    // TODO
}
