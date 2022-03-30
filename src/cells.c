#include <stdlib.h>
#include <stdio.h>
#include "keys.h"
#include "cells.h"


// CellKey

CellKey *create_cell_key(Key *key)
{
    CellKey *rep = malloc(sizeof(CellKey));
    rep->data = key;
    rep->next = NULL;
    return rep;
}

CellKey* read_public_keys(char* name){
    // TODO
}

void add_cell_key(CellKey **l, Key *k)
{
    CellKey *rep = create_cell_key(k);
    rep->next = *l;
    *l = rep;
}

void print_list_keys(CellKey *LCK)
{
    printf("[ ");
    for (; LCK; LCK = LCK->next)
        printf("%s ", key_to_str(LCK->data));
    printf("]\n");
}

void delete_cell_key(CellKey *c)
{
    free_key(c->data);
    free(c);
}

void delete_list_keys(CellKey *c)
{
    if (!c)
        return;
    CellKey *temp;
    do
    {
        temp = c->next;
        delete_cell_key(c);
        c = temp;
    } while (c);
}

// CellProtected

CellProtected* create_cell_protected(Protected* pr)
{
    CellProtected* rep = malloc(sizeof(CellProtected));
    rep->data = pr;
    rep->next = NULL;
    return rep;
}

void add_cell_protected(CellProtected** c, Protected* p)
{
    CellProtected* rep = create_cell_protected(p);
    rep->next = *c;
    *c = rep;
}

CellProtected* read_protected(char* name)
{
    // TODO
}


void print_list_protected(CellProtected *l)
{
    printf("[ ");
    for (; l; l = l->next)
        printf("%s ", protected_to_str(l->data));
    printf("]\n");
}

void delete_cell_protected(CellProtected* c)
{
    free_protected(c->data);
    free(c);
}


void delete_list_protected(CellProtected *c)
{
    if (!c)
        return;
    CellKey *temp;
    do
    {
        temp = c->next;
        delete_cell_protected(c);
        c = temp;
    } while (c);
}
