#include "utils_tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_long_vector(long *result, int size)
{
    printf(" [ ");
    for (int i = 0; i < size; i++)
    {
        printf("%ld \t ", result[i]);
    }
    printf("] \n");
}

// they must finish with \0, 1 if == else 0
// <!> doesn't work if 0 in array <!>
int longl_cmp(long *l1, long *l2)
{
    for (; *l1 && *l2 && *l1 == *l2; l1++, l2++)
        ;
    return *l1 == '\0' && *l2 == '\0';
}

Key *create_key(long val, long n)
{
    Key *k = malloc(sizeof(Key));
    init_key(k, val, n);
    return k;
}

CellKey *generate_cellkeys(int s)
{
    CellKey *rep = NULL;
    for (int i = 0; i < s; i++)
        add_cell_key(&rep, create_key(rand()/2, rand()/2));
    return rep;
}

// genere une liste semi_valide <!>
// la première est toujours non valide !
CellProtected *generate_cellprotected(int s, char *msg)
{
    CellProtected *rep = NULL;
    Key *k, *k2;
    Signature *si;
    for (int i = 0; i < s - 1; i++)
    {
        if (rand() % 2)
        {
            add_cell_protected(
                &rep,
                init_protected(
                    create_key(
                        rand()/2,
                        rand()/2),
                    msg,
                    init_signature(NULL, 0)));
        }
        else
        {
            k = malloc(sizeof(Key));
            k2 = malloc(sizeof(Key));
            init_pair_keys(k, k2, 100, 1000);
            si = sign(msg, k2);
            add_cell_protected(
                &rep,
                init_protected(
                    k,
                    msg,
                    si));
            free_key(k2);
        }
    }
    add_cell_protected(
        &rep,
        init_protected(
            create_key(
                rand()/2,
                rand()/2),
            msg,
            init_signature(NULL, 0)));
    return rep;
}

Signature* dup_sign(Signature* s)
{
    return init_signature(s->content, s->size);
}

Key* dup_key(Key* k)
{
    return create_key(k->val, k->n);
}

Block* generate_block(int s, char* msg ,char* hash, char* prev)
{
    Block *b = malloc(sizeof(Block));
    b->author = create_key(0,0);
    b->votes = generate_cellprotected(s, msg);
    b->nonce = 0;
    b->hash = (unsigned char *)strdup(hash);
    b->previous_hash = (unsigned char *)strdup(prev);
    return b;
}