#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int is_prime_naive(long p)
{
    // max under 2 ms p=5000
    int s = (int)sqrt(p);
    int is_prime = 1;
    for (int i = 3; i < s; i++)
        if (!(p % s))
            is_prime = 0;
    return is_prime;
}
long modpow_naive(long a, long m, long n)
{
    long int rep = 1;
    for (long int i = 0; i < m; i++)
        rep = (rep * a) % n;
    return rep;
}

long modpow(long a, long m, long n)
{
    long int rep = a;
    for (long int i = 1; i < m; i *= 2)
        rep = (rep * rep) % n;
    if (m % 2 && m > 2)
        return (rep * a) % n;
    return rep;
}

int witness(long a, long b, long d, long p)
{
    long x = modpow(a, d, p);
    if (x == 1)
        return 0;
    for (long i = 0; i < b; i++)
    {
        if (x == p - 1)
            return 0;
        x = modpow(x, 2, p);
    }
    return 1;
}
long rand_long(long low, long up)
{
    return rand() % (up - low + 1) + low;
}

int is_prime_miller(long p, int k)
{
    if (p == 2)
    {
        return 1;
    }
    if (!(p & 1) || p <= 1)
    { // on verifie que p est impair et different de 1
        return 0;
    }
    // on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!(d & 1))
    { // tant que d n’est pas impair
        d = d / 2;
        b = b + 1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a;
    int i;
    for (i = 0; i < k; i++)
    {
        a = rand_long(2, p - 1);
        if (witness(a, b, d, p))
        {
            return 0;
        }
    }
    return 1;
}

long random_prime_number(int low_size, int up_size, int k)
{
    long low = 1 << (low_size - 1);
    long high = (1 << up_size) - 1;
    long cand;
    int isprime = 0;
    while (!isprime)
    {
        cand = rand_long(low_size, up_size);
        isprime = is_prime_miller(cand, k);
    }
    return cand;
}

long extended_gcd(long s, long t, long *u, long *v)
{
    if (s == 0)
    {
        *u = 0;
        *v = 1;
        return t;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t % s, s, &uPrim, &vPrim);
    *u = vPrim - (t / s) * uPrim;
    *v = uPrim;
    return gcd;
}