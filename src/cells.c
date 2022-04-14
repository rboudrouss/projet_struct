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

CellKey *read_public_keys(char *name)
{
    // reads public key from file
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

CellProtected *create_cell_protected(Protected *pr)
{
    CellProtected *rep = malloc(sizeof(CellProtected));
    rep->data = pr;
    rep->next = NULL;
    return rep;
}

void add_cell_protected(CellProtected **c, Protected *p)
{
    CellProtected *rep = create_cell_protected(p);
    rep->next = *c;
    *c = rep;
}

CellProtected *read_protected(char *name)
{
    // reads signature key from file
    // TODO
}

void print_list_protected(CellProtected *l)
{
    printf("[ ");
    char *s;
    for (; l; l = l->next){
        s = protected_to_str(l->data);
        printf("%s ",s);
        free(s);
    }
    printf("]\n");
}

void delete_cell_protected(CellProtected *c)
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

void delete_non_valid(CellProtected **c)
{
    if (!*c)
        return;

    CellProtected *temp = *c, *suiv;
    int verified = verify(temp->data);

    while (!verified)
    {
        temp = temp->next;
        (*c);
        c = &temp;
        verified = verify(temp->data);
    }

    while (temp->next)
    {
        if (!verify(temp->next))
        {
            suiv = temp->next->next;
            delete_cell_protected(temp->next);
            temp->next = suiv;
        }
        else
            temp = temp->next;
    }
}

HashCell *create_hashcell(Key *key)
{
    HashCell *rep = malloc(sizeof(HashCell));
    rep->key = key;
    rep->val = 0;
    return rep;
}

int hash_function(Key *key, int size)
{
    if (!key)
    {
        printf("no key specified for hash_fucntion\n");
        return;
    }
    return (key->n + key->val) % size;
}

int find_position(HashTable *t, Key *key)
{
    if (!(t && key))
        return;
    HashCell **tab = t->tab;
    int hash = hash_function(key, t->size);
    for (; tab[hash] && !key_cmp(tab[hash]->key, key); hash = (hash + 1) % t->size)
        ;
    return hash;
}

void insert_key_table(HashTable *t, Key *key)
{
    HashCell *c = create_hashcell(key);
    int pos = find_position(t, key);
    t->tab[pos] = c;
}

HashCell *get_cell_table(HashTable *t, Key *key)
{
    return t->tab[find_position(t, key)];
}

HashTable *create_hashtable(CellKey *keys, int size)
{
    HashTable *rep = malloc(sizeof(HashTable));
    rep->size = size;
    rep->tab = malloc(sizeof(sizeof(CellKey *) * size));

    for (int i = 0; i < size; rep->tab[i++] = NULL)
        ;

    for (; keys; keys = keys->next)
        insert_key_table(rep, keys->data);

    return rep;
}

void delete_hashtable(HashTable *t)
{
    free(t->tab);
    free(t);
}

Key *compute_winner(CellProtected *decl, CellKey *candidates, CellKey *voters, int sizeC, int sizeV)
{
    HashTable *HC = create_hashtable(candidates, sizeC);
    HashTable *HV = create_hashtable(voters, sizeV);

    Protected *p;
    HashCell *c;
    Key *keyc;
    int nb_vote = -1;

    for (; decl; decl = decl->next)
    {
        p = decl->data;
        c = get_cell_table(HV, decl->data->pKey);

        if (!c)
        {
            printf("Warning : Candidate not found in compute winner\n");
            continue;
        }

        if (c->val == 1)
        {
            printf("Warning : candidate has already voted, there might be a duplicate in the array\n");
            continue;
        }

        keyc = str_to_key(decl->data->mess);
        c = get_cell_table(HC, keyc);

        if (!c)
        {
            printf("Warning : Signature to an non-existing candidate\n");
            free_key(keyc);
            keyc = NULL;
            continue;
        }

        c->val++;
        free_key(keyc);
        keyc = NULL;
    }

    for (int i = 0; i < sizeC; i++)
    {
        if (!HC->tab[i])
            continue;
        if (HC->tab[i]->val > nb_vote)
        {
            nb_vote = HC->tab[i]->val;
            keyc = HC->tab[i]->key;
        }
    }
    return keyc;
}