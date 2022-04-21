#include <stdlib.h>
#include <stdio.h>
#include "generate.h"
#include "files.h"

void generate_random_data(int nv, int nc, int np)
{ // TODO séparer cetet fonction en pls petites fonctions
    if (!nv)
        return;
    FILE *keysf = fopen(KEYSF, "wb");
    Key *pk = malloc(sizeof(Key)), *sk = malloc(sizeof(Key));
    Key **keyp = malloc(sizeof(Key *) * nv);
    Key **keys = malloc(sizeof(Key *) * nv);
    Key **keyc = malloc(sizeof(Key *) * nc);
    int r;
    char *str1, *str2;
    // génére les votants
    for (int i = 0; i < nv; i++)
    {
        // TODO implémenter anti-duplicata
        init_pair_keys(pk, sk, 100, 1000);
        keyp[i] = malloc(sizeof(Key));
        keys[i] = malloc(sizeof(Key));
        init_key(keyp[i], pk->val, pk->n);
        init_key(keys[i], sk->val, sk->n);
        str1 = key_to_str(pk);
        str2 = key_to_str(sk);
        fprintf(keysf, "%s %s\n", str1, str2);
        free(str1);
        free(str2);
    }

    free_key(pk);
    free_key(sk);
    fclose(keysf);

    if (!nc)
    {
        // TODO free memory
        return;
    }
    // génére des candidats aléatoires
    FILE *candf = fopen(CANDF, "w");
    for (int i = 0; i < nc; i++)
    {
        // TODO implémenter anti-duplicata
        keyc[i] = malloc(sizeof(Key));
        r = rand() % nv;
        init_key(keyc[i], keyp[r]->val, keyp[r]->n);
        str1 = key_to_str(keyp[r]);
        fprintf(candf, "%s\n", str1);
        free(str1);
    }
    fclose(candf);

    // génére les signatures/déclarations
    FILE *decf = fopen(DECF, "w");
    Signature *s;
    Protected *p;
    for (int i = 0; i < nv - np; i++)
    {
        r = rand() % nc;
        str1 = key_to_str(keyc[r]);
        s = sign(str1, keys[i]);
        p = init_protected(keyp[i], str1, s);
        str2 = protected_to_str(p);
        fprintf(decf, "%s\n", str2);
        free(str1);
        free(str2);
        free_protected(p);
    }
    fclose(decf);

    // generate random pending vote
    for (int i = nv - np; i < nv; i++)
    {
        r = rand() % nc;
        str1 = key_to_str(keyc[r]);
        s = sign(str1, keys[i]);
        p = init_protected(keyp[i], str1, s);
        submit_vote(p); // using function for standardization but slower
        free(str1);
        free_protected(p);
    }
    // TODO free tout le tableau (boucler sur chaque tableau et free)
    /*for (int i = 0; i < nv; i++)
    {
        free_key(keyp[i]);
        free_key(keys[i]);
    }
    for (int i = 0; i < nc; i++)
        free_key(keyc[i]);
    */
}