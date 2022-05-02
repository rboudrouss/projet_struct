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
    for (; cell; cell = cell->firstChild)
    {
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
        b->previous_hash = (unsigned char *)strdup((char *)last_node(tree)->block->hash);
    else
        b->previous_hash = (unsigned char *)strdup("null");
    b->votes = pl;
    compute_proof_of_work(b, d);
    write_block(PENDB, b);

    b->author = NULL; // HACK
    delete_block(b);
}

void add_block(int d, char *name)
{
    Block *b = read_block(PENDB);
    // FILE *pbf = fopen(PENDB, "w");
    // fputc('\n', pbf);
    // fclose(pbf);

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
    DIR *rep = opendir(BCFOLDER);
    struct dirent *dir;

    int file_no = 0, i = 0;

    if (rep != NULL)
    {
        // Counting all files
        while ((dir = readdir(rep)))
        {
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") && strcmp(dir->d_name, "temp"))
            { // Neither . nor ..
                file_no++;
            }
        }
    }
    else
        return NULL;

    CellTree *node_arr[file_no];

    rewinddir(rep);
    chdir(BCFOLDER); // Moving to the correct directory
    while ((dir = readdir(rep)))
    {
        if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") && strcmp(dir->d_name, "temp"))
        { // Neither . nor ..
            Block *block = read_block(dir->d_name);
            node_arr[i] = create_node(block); // Will break
            i++;
        }
    }

    closedir(rep);
    chdir("..");

    for (i = 0; i < file_no; i++)
    {
        for (int j = 0; j < file_no; j++)
        {
            if (i == j)
                continue; // Do not compare with self

            if ((node_arr[j]->block->previous_hash) && (strcmp((char *)node_arr[i]->block->hash, (char *)node_arr[j]->block->previous_hash) == 0))
            {
                add_child(&node_arr[i], node_arr[j]);
            }
        }
    }

    CellTree *root = NULL;
    for (int i = 0; i < file_no; i++)
    {
        if (node_arr[i]->father == NULL)
            root = node_arr[i];
    }

    if (root == NULL)
        perror("Returning NULL in read_tree.\n");
    return root;
}

Key *compute_winner_BT(CellTree *tree, CellKey *candidates, CellKey *voters, int sizeC, int sizeV)
{
    CellProtected *votes = fusion_blocks(tree);
    delete_non_valid(&votes);

    Key *winner = compute_winner(votes, candidates, voters, sizeC, sizeV);

    while (votes != NULL)
    {
        CellProtected *temp = votes;
        votes = votes->next;
        free(temp);
    }

    return winner;
}

void free_all_protected_in_tree(CellTree *tree)
{
    while (tree != NULL)
    {

        CellProtected *votes = tree->block->votes;
        while (votes != NULL)
        {
            free_protected(votes->data);
            votes = votes->next;
        }

        tree = tree->firstChild;
    }
}

void free_all_authors_in_tree(CellTree *tree)
{
    while (tree != NULL)
    {
        free(tree->block->author);
        tree = tree->firstChild;
    }
}