#include <stdlib.h>
#include <stdio.h>
#include "rsa.h"
#include "prime.h"
#include "keys.h"

void init_key(Key *key, long val, long n)
{
    key->val = val;
    key->n = n;
}

void free_key(Key *key)
{
    free(key);
}

// 1 if same else 0
int key_cmp(Key *k1, Key *k2)
{
    if(!(k1&&k2))
        return 0;
    return k1->n == k2->n && k1->val == k2->val;
}

void init_pair_keys(Key *pKey, Key *sKey, long low_size, long up_size)
{

    long n, s, u;
    long p = random_prime_number(low_size, up_size, 5000);
    long q = random_prime_number(low_size, up_size, 5000);
    while (p == q)
        q = random_prime_number(low_size, up_size, 5000);
    generate_key_values(p, q, &n, &s, &u);

    if (u < 0)
    {
        long t = (p - 1) * (q - 1);
        u = u + t;
    }
    pKey->n = sKey->n = n;
    pKey->val = s;
    sKey->val = u;
}

char *key_to_str(Key *key)
{
    char *rep = malloc(STR_SIZE * sizeof(char));
    sprintf(rep, "(%lx,%lx)", key->val, key->n);
    return rep;
}

Key *str_to_key(char *str)
{
    if(!str)
        return NULL;
    Key *k = malloc(sizeof(Key));
    long val, n;
    sscanf(str, "(%lx,%lx)", &val, &n);
    k->val = val;
    k->n = n;
    return k;
}
