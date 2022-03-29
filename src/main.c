#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "prime.h"
#include "rsa.h"

void print_long_vector(long *result, int size)
{
    printf(" Vector : [ ");
    for (int i = 0; i < size; i++)
    {
        printf("%lx \t ", result[i]);
    }
    printf("] \n");
}

void print_str_as_int(char *s)
{
    printf("[ ");
    for (; *s; s++)
        printf("%d ", (int)*s);
    printf("0 ]\n");
}

int main()
{
    srand(time(NULL));

    // Generation de cle : $(OH)
    long p = random_prime_number(3, 7, 5000);
    long q = random_prime_number(3, 7, 5000);
    printf("nyah %ld %ld \n", p, q);
    while (p == q)
        q = random_prime_number(3, 7, 5000);
    printf("nyah\n");
    long n, s, u;
    generate_key_values(p, q, &n, &s, &u);
    // Pour avoir des cles positives :
    if (u < 0)
    {
        long t = (p - 1) * (q - 1);
        u = u + t; // on aura toujours s*u mod t = 1
    }

    // Afichage des cles en hexadecimal
    printf("cle publique = (%ld , %ld ) \n", s, n);
    printf("cle privee = (%ld , %ld ) \n", u, n);

    // Chiffrement:
    char mess[10] = "Hello";
    print_str_as_int(mess);
    int len = strlen(mess);
    long *crypted = encrypt(mess, s, n);

    printf("Initial message : %s \n", mess);
    printf("Encoded representation : \n");
    print_long_vector(crypted, len);

    // Dechiffrement
    char *decoded = decrypt(crypted, len, u, n);
    printf(" Decoded : %s \n", decoded);

    return 0;
}