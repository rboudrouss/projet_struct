#include <stdlib.h>
#include <stdio.h>
#include "generate.h"
#include "files.h"
#include <assert.h>

Key *get_key_el(CellKey *l, int i)
{
    for (int j = 0; l && j < i; j++)
        l = l->next;
    return l ? l->data : NULL;
}

int get_length(CellKey *l)
{
    int i;
    for (i = 0; l; i++)
        l = l->next;
    return i - 1;
}

void generate_random_data(int nv, int nc, int np)
{
    printf("Generating data...\n");
    printf("Generating %d random keys...\n", nv);
    generate_random_keys(nv);
    printf("Generating %d random candidates...\n", nc);
    generate_random_cand(nc);
    printf("Generating %d random signatures...\n", nv - np);
    generate_signature(nv - np);
    printf("Generating pending %d votes...\n", np);
    generate_pending(nv - np, np);
    printf("All data generated !\n");
}

void generate_random_keys(int nv)
{
    if (nv < 1)
        return;
    FILE *keysf = fopen(KEYSF, "wb");
    Key *pk = malloc(sizeof(Key)), *sk = malloc(sizeof(Key));
    char *str1, *str2;
    // génére les votants
    for (int i = 0; i < nv; i++)
    {
        // TODO implémenter anti-duplicata
        init_pair_keys(pk, sk, 100, 1000);
        str1 = key_to_str(pk);
        str2 = key_to_str(sk);
        fprintf(keysf, "%s %s\n", str1, str2);
        free(str1);
        free(str2);
    }

    free_key(pk);
    free_key(sk);
    fclose(keysf);
}

void generate_random_cand(int nc)
{
    CellKey *keys = read_public_keys_fromp(KEYSF);
    int nv = get_length(keys);
    int r;
    char *str;

    FILE *candf = fopen(CANDF, "w");
    for (int i = 0; i < nc; i++)
    {
        r = rand() % nv;
        str = key_to_str(get_key_el(keys, r));
        fprintf(candf, "%s\n", str);
        free(str);
    }
    fclose(candf);
    delete_list_keys(keys);
}

void generate_signature(int ns)
{
    CellKey *pkeys = read_public_keys_fromp(KEYSF);
    CellKey *skeys = read_secret_keys_fromp(KEYSF);

    CellKey *ckeys = read_public_keys(CANDF);

    int nv = get_length(pkeys);
    int nc = get_length(ckeys);

    assert(ns <= nv);

    int r;
    char *str1, *str2;

    // génére les signatures/déclarations
    FILE *decf = fopen(DECF, "w");
    Signature *s;
    Protected *p;
    for (int i = 0; i < ns; i++)
    {
        r = rand() % nc;
        str1 = key_to_str(get_key_el(ckeys, r));
        s = sign(str1, get_key_el(skeys, i));
        p = init_protected(get_key_el(pkeys, i), str1, s);
        str2 = protected_to_str(p);
        fprintf(decf, "%s\n", str2);
        free(str1);
        free(str2);
        free(p->mess);
        free_signature(p->sgn);
        free(p);
    }
    fclose(decf);
    delete_list_keys(pkeys);
    delete_list_keys(skeys);
    delete_list_keys(ckeys);
}

void generate_pending(int ns, int np)
{
    CellKey *pkeys = read_public_keys_fromp(KEYSF);
    CellKey *skeys = read_secret_keys_fromp(KEYSF);

    CellKey *ckeys = read_public_keys(CANDF);

    int nv = get_length(pkeys);
    int nc = get_length(ckeys);

    assert(np + ns <= nv);

    int r;
    char *str1;

    // génére pendings votes
    Signature *s;
    Protected *p;
    for (int i = ns; i < np; i++)
    {
        r = rand() % nc;
        str1 = key_to_str(get_key_el(ckeys, r));
        s = sign(str1, get_key_el(skeys, i));
        p = init_protected(get_key_el(pkeys, i), str1, s);
        submit_vote(p); // using function for standardization but slower
        free(str1);
        free(p->mess);
        free_signature(p->sgn);
        free(p);
    }
    delete_list_keys(pkeys);
    delete_list_keys(skeys);
    delete_list_keys(ckeys);
}

void generate_random_pending(int np)
{
    CellKey *pkeys = read_public_keys_fromp(KEYSF);
    CellKey *skeys = read_secret_keys_fromp(KEYSF);

    CellKey *ckeys = read_public_keys(CANDF);

    int nv = get_length(pkeys);
    int nc = get_length(ckeys);

    int rc, rv;
    char *str1;

    // génére pendings votes
    Signature *s;
    Protected *p;
    for (int i = 0; i < np; i++)
    {
        rc = rand() % nc;
        rv = rand() % nv;
        str1 = key_to_str(get_key_el(ckeys, rc));
        s = sign(str1, get_key_el(skeys, rv));
        p = init_protected(get_key_el(pkeys, rv), str1, s);
        submit_vote(p); // using function for standardization but slower
        free(str1);
        free(p->mess);
        free_signature(p->sgn);
        free(p);
    }
    delete_list_keys(pkeys);
    delete_list_keys(skeys);
    delete_list_keys(ckeys);
}