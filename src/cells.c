#include <stdlib.h>
#include <stdio.h>
#include "keys.h"
#include "cells.h"
#include <string.h>

// CellKey

CellKey *create_cell_key(Key *key)
{
    CellKey *rep = malloc(sizeof(CellKey));
    rep->data = key;
    rep->next = NULL;
    return rep;
}

// read from pair of public & private key
// file must finish with a \n
CellKey *read_public_keys_fromp(char *name)
{
    FILE *f = fopen(name, "r");
    char buffer1[STR_SIZE], buffer2[STR_SIZE];
    fscanf(f, "%s %s\n", buffer1, buffer2);
    Key *k = str_to_key(buffer1);
    CellKey *kl = create_cell_key(k);
    while (!feof(f))
    {
        fscanf(f, "%s %s", buffer1, buffer2);
        k = str_to_key(buffer1);
        add_cell_key(&kl, k);
    };
    fclose(f);
    return kl;
}
// read from pair of public & private key
// file must finish with a \n
CellKey *read_secret_keys_fromp(char *name)
{
    FILE *f = fopen(name, "r");
    char buffer1[STR_SIZE], buffer2[STR_SIZE];
    fscanf(f, "%s %s\n", buffer1, buffer2);
    Key *k = str_to_key(buffer1);
    CellKey *kl = create_cell_key(k);
    while (!feof(f))
    {
        fscanf(f, "%s %s", buffer1, buffer2);
        k = str_to_key(buffer2);
        add_cell_key(&kl, k);
    };
    fclose(f);
    return kl;
}

// FIXME duplicate candidant (?)
// file must finish with a \n
CellKey *read_public_keys(char *name)
{
    FILE *f = fopen(name, "r");
    char buffer[STR_SIZE];
    fgets(buffer, sizeof(buffer), f);
    Key *p = str_to_key(buffer);
    CellKey *pl = create_cell_key(p);
    while (!feof(f))
    {
        fgets(buffer, sizeof(buffer), f);
        if (feof(f))
            break; // HACK
        p = str_to_key(buffer);
        add_cell_key(&pl, p);
    }
    fclose(f);
    return pl;
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
    char *s;
    for (; LCK; LCK = LCK->next)
    {
        if (LCK->data)
        {
            s = key_to_str(LCK->data);
            printf("%s ", s);
            free(s);
        }
        else
            printf("END");
    }
    printf(" ]\n");
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

void reversecp(CellProtected *current, CellProtected *previous, CellProtected **head)
{
    if (!current->next)
    {
        *head = current;
        current->next = previous;
        return;
    }
    CellProtected *next = current->next;
    current->next = previous;
    reversecp(next, current, head);
}

void reverse_cell_protected(CellProtected **head)
{
    if (!head)
        return;
    reversecp(*head, NULL, head);
}

CellProtected *read_protected(char *filename)
{
    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier (read_protected).\n");
        return NULL;
    }

    char pr_str[81], temp1[64], temp2[128];
    Protected *pr;
    CellProtected *liste = NULL;
    while (!feof(f))
    {
        fscanf(f, "%s %s %s\n", pr_str, temp1, temp2);
        if (!(*pr_str && *temp1 && *temp2))
            continue;

        char *temp12 = strdup(temp1); // HACK
        char *temp22 = strdup(temp2);
        char *full_str = strcat(strcat(strcat(strcat(pr_str, " "), temp12), " "), temp22);
        free(temp12);
        free(temp22);
        pr = str_to_protected(full_str);
        add_cell_protected(&liste, pr);
    }

    fclose(f);

    return liste;
}

void print_list_protected(CellProtected *l)
{
    printf("[ ");
    char *s;
    for (; l; l = l->next)
    {
        if (l->data)
        {
            s = protected_to_str(l->data);
            printf("%s", s);
            free(s);
        }
        else
            printf("<!>EMPTY<!>");
        if (l->next)
            printf(" || ");
    }
    printf(" ]\n");
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

    CellProtected *temp;

    do
    {
        temp = c->next;
        delete_cell_protected(c);
        c = temp;
    } while (c);
}

// doesn't delete Keys
void delete_only_list_protected(CellProtected *c)
{
    if (!c)
        return;

    CellProtected *temp;

    do
    {
        temp = c->next;
        free(c->data->mess);
        free_signature(c->data->sgn);
        free(c->data);
        free(c);
        c = temp;
    } while (c);
}

void only_free_list_protected(CellProtected *l)
{
    CellProtected *t;
    while (l)
    {
        t = l->next;
        free(l);
        l = t;
    }
}

void delete_non_valid(CellProtected **c)
{
    if (!*c)
        return;

    CellProtected *temp = *c, *suiv;
    int verified = verify(temp->data);

    while (!verified && temp)
    {
        temp = temp->next;
        delete_cell_protected(*c);
        *c = temp;
        verified = verify(temp->data);
    }
    if (!temp)
        return;

    while (temp->next)
    {
        if (!verify(temp->next->data))
        {
            suiv = temp->next->next;
            delete_cell_protected(temp->next);
            temp->next = suiv;
        }
        else
            temp = temp->next;
    }
}

CellProtected *fusion_protected(CellProtected *p1, CellProtected *p2)
{
    CellProtected *chaine = NULL;

    while (p1 != NULL)
    {
        add_cell_protected(&chaine, p1->data);
        p1 = p1->next;
    }

    while (p2 != NULL)
    {
        add_cell_protected(&chaine, p2->data);
        p2 = p2->next;
    }

    return chaine;
}

// hashtab

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
        printf("Error : no key specified for hash_fucntion\n");
        return 0;
    }
    return (key->n + key->val) % size;
}

int find_position(HashTable *t, Key *key)
{
    if (!t)
    {
        printf("Error : no table specified of find_postion function\n");
        return 0;
    }
    if (!key)
    {
        printf("Error : no key specified of find_postion function\n");
        return 0;
    }
    HashCell **tab = t->tab;
    int hash = hash_function(key, t->size);

    int i;
    for (i = 0; tab[hash] && !key_cmp(tab[hash]->key, key) && i < t->size + 1; hash = (hash + 1) % t->size, i++)
        ;
    if (i > t->size)
    {
        printf("<!> HASHTABLE IS FULL <!>\n");
        return 0;
    }
    return hash;
}

void insert_key_table(HashTable *t, Key *key)
{
    int pos = find_position(t, key);
    if(t->tab[pos])
        return;
    HashCell *c = create_hashcell(key);
    t->tab[pos] = c;
}

// get value by key
HashCell *get_cell_table(HashTable *t, Key *key)
{
    if (!key)
    {
        printf("Error : got empty key in get_cell_table\n");
        return NULL;
    }
    return t->tab[find_position(t, key)];
}

HashTable *create_hashtable(CellKey *keys, int size)
{
    HashTable *rep = malloc(sizeof(HashTable));
    rep->size = size;
    rep->tab = malloc(sizeof(CellKey *) * size);

    for (int i = 0; i < size; i++)
        rep->tab[i] = NULL;

    for (; keys; keys = keys->next)
        insert_key_table(rep, keys->data);

    return rep;
}

void delete_hashtable(HashTable *t)
{
    for (int i = 0; i < t->size; i++)
        if (t->tab[i])
            free(t->tab[i]);
    free(t->tab);
    free(t);
}

void print_hashtable(HashTable *t)
{
    char *s;
    printf("Printing hastable : [\n");
    for (int i = 0; i < t->size; i++)
        if (t->tab[i])
        {
            s = key_to_str(t->tab[i]->key);
            printf("%03d | %s : %d\n", i, s, t->tab[i]->val);
            free(s);
        }

    printf("]\n");
}

Key *compute_winner(CellProtected *decl, CellKey *candidates, CellKey *voters, int sizeC, int sizeV)
{
    HashTable *HC = create_hashtable(candidates, sizeC * 2);
    HashTable *HV = create_hashtable(voters, sizeV * 2);

    HashCell *c, *v;
    Key *keyc;

    for (; decl; decl = decl->next)
    {
        v = get_cell_table(HV, decl->data->pKey);

        if (!v)
        {
            printf("Warning : voter not found in compute winner\n");
            continue;
        }

        if (v->val == 1)
        {
            printf("Warning : voter has already voted, there might be a duplicate in the array\n");
            continue;
        }
        v->val = 1;

        keyc = str_to_key(decl->data->mess);
        c = get_cell_table(HC, keyc);
        free_key(keyc);
        keyc = NULL;

        if (!c)
        {
            printf("Warning : Signature to an non-existing candidate, mess = %s\n", decl->data->mess);
            continue;
        }
        c->val++;
    }

    int nb_vote = -1;

    for (int i = 0; i < sizeC * 2; i++)
    {
        if (!HC->tab[i])
            continue;
        if (HC->tab[i]->val > nb_vote)
        {
            nb_vote = HC->tab[i]->val;
            keyc = HC->tab[i]->key;
        }
    }
    printf("Le gagnant remport l'élection avec %d voix !\n", nb_vote);
    delete_hashtable(HC);
    delete_hashtable(HV);
    return keyc;
}