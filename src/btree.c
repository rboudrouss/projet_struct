#include <stdlib.h>
#include <stdio.h>
#include "btree.h"
#include "files.h"
#include <dirent.h>
#include <string.h>
#define COUNT 10

CellTree *init_father_tree(CellProtected *c, int d)
{
    CellTree *rep = malloc(sizeof(CellTree));
    Block *b = malloc(sizeof(Block));
    b->previous_hash = (unsigned char *)"\0";
    b->author = str_to_key("(0,0)");
    b->votes = c;
    compute_proof_of_work(b, d);
    rep->block = b;
    rep->father = rep->firstChild = rep->nextBro = NULL;
    rep->height = 0;
    return rep;
}

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
    if (!father)
    {
        printf("Error : no father in update_height function, not possible\n");
        return -1;
    }
    if (!child)
    {
        int rep = father->height == 0;
        father->height = 0;
        return rep;
    }
    if (father->height > (child->height + 1))
        return 0;
    father->height = child->height + 1;
    return 1;
}

void add_child(CellTree **father, CellTree *child)
{
    if (!*father)
    {
        *father = child;
        return;
    }
    CellTree *f = *father;
    CellTree *temp = f->firstChild;
    f->firstChild = child;
    child->nextBro = temp;
    child->father = f;

    for (; child->father; child = child->father)
        update_height(child->father, child);
}

void print_tree(CellTree *tree)
{
    if (tree == NULL)
        return;

    printf("\nHauteur %d : %s\n", tree->height, tree->block->hash);
    printf("\tself : %p\n\tfather : %p\n\tfirstChild : %p\n\tnextBro : %p\n\tprevious_hash : %s\n", tree, tree->father, tree->firstChild, tree->nextBro, (char *)tree->block->previous_hash);

    print_tree(tree->nextBro);
    print_tree(tree->firstChild);
}

void delete_node(CellTree *node)
{
    if (!node)
        return;
    delete_block(node->block);
    free(node);
}

void delete_tree(CellTree *tree)
{
    if (!tree)
        return;
    delete_tree(tree->firstChild);
    delete_tree(tree->nextBro);

    delete_block(tree->block);
    free(tree);
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
    CellProtected *rep = NULL, *new;
    for (; cell; cell = cell->firstChild){
        new = fusion_protected(rep, cell->block->votes);
        only_free_list_protected(rep);
        rep = new;
    }
    return rep;
}

void create_block(CellTree *tree, Key *author, int d)
{
    CellProtected *pl = read_protected(PENDV);
    delete_non_valid(&pl);
    FILE *pf = fopen(PENDV, "w");
    fputc('\n', pf);
    fclose(pf);

    Block *b = malloc(sizeof(Block));
    b->author = author;
    if (tree)
        b->previous_hash = last_node(tree)->block->hash;
    else
        b->previous_hash =(unsigned char*) strdup("null");
    b->votes = pl;
    compute_proof_of_work(b, d);
    write_block(PENDB, b);

    delete_block(b);
}

void add_block(int d, char *name)
{
    Block *b = read_block(PENDB);
    FILE *pbf = fopen(PENDB, "w");
    fputc('\n', pbf);
    fclose(pbf);

    if (!verify_block(b, d))
    {
        printf("block non valid\n");
        delete_block(b);
        return;
    }
    write_block(name, b);
    delete_block(b);
}

CellTree *read_tree()
{
    // FIXME erreur lors de l'ouverture du fichiers
    DIR *rep = opendir(BCFOLDER);
    Block *b;
    CellTree *c = NULL, *c2;
    if (rep != NULL)
    {
        struct dirent *dir;
        while ((dir = readdir(rep)))
        {
            if ((strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") && strcmp(dir->d_name, "temp")))
            {
                b = read_block(strcat(BCFOLDER "/", dir->d_name));
                c2 = create_node(b);
                add_child(&c, c2);
            }
        }
        closedir(rep);
    }
    return c;
}