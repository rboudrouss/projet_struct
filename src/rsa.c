#include <string.h>
#include <stdlib.h>
#include "prime.h"

void generate_key_values(long p, long q, long *n, long *s, long *u)
{
    *n = p * q;
    long t = (p - 1) * (q - 1);
    long v, gcd;
    do
    {
        *s = rand_long(2, t);
        gcd = extended_gcd(*s, t, u, &v);
    } while (gcd != 1);
}

long *encrypt(char *chaine, long s, long n)
{
    size_t len = strlen(chaine);
    long *rep = malloc(sizeof(long) * len);
    for (int i = 0; i < len - 1; i++)
        rep[i] = modpow((long)chaine[i], s, n);
    rep[len - 1] = '\0';
    return rep;
}

char *decrypt(long *crypted, int size, long u, long n)
{
    char *rep = malloc(size * sizeof(char));
    for (int i = 0; i < size - 1; i++)
        rep[i] = (char)modpow(crypted[i], u, n);
    rep[size - 1] = '\0';
    return rep;
}
