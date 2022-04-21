#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "block.h"
#include "rsa.h"
#include "prime.h"

void print_long_vector(long *result, int size)
{
    printf(" Vector : [ ");
    for (int i = 0; i < size; i++)
    {
        printf("%ld \t ", result[i]);
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
int main(void)
{

    srand(time(NULL));
    // Generation de cle : $(OH)
    long p = random_prime_number(8, 20, 5000);
    long q = random_prime_number(8, 20, 5000);
    while (p == q)
        q = random_prime_number(8, 20, 5000);
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
    char mess[100] = "j'aime le fromage uwu";
    print_str_as_int(mess);
    int len = strlen(mess);
    long *crypted = encrypt(mess, s, n);

    printf("Initial message : %s \n", mess);
    printf("Encoded representation : \n");
    print_long_vector(crypted, len);

    // Dechiffrement
    char *decoded = decrypt(crypted, len, u, n);
    printf(" Decoded : %s \n", decoded);

    // Testing Init Keys
    Key *pKey = malloc(sizeof(Key));
    Key *sKey = malloc(sizeof(Key));
    init_pair_keys(pKey, sKey, 239, 331);
    printf("pKey : %lx , %lx \n", pKey->val, pKey->n);
    printf(" sKey : %lx , %lx  \n", sKey->val, sKey->n);

    // Testing Key Serialization
    char *chaine = key_to_str(pKey);
    printf("keytostr: %s \n", chaine);
    Key *k = str_to_key(chaine);
    printf("strtokey: %lx , %lx \n", k->val, k->n);

    // Testing signature
    // Candidate keys:
    Key *pKeyC = malloc(sizeof(Key));
    Key *sKeyC = malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC, 197, 331);
    // Declaration:
    char *mess1 = key_to_str(pKeyC);
    printf("%s vote pour %s \n", key_to_str(pKey), mess1);

    Signature *sgn = sign(mess1, sKey);
    printf("signature: ");
    print_long_vector(sgn->content, sgn->size);

    free(chaine);

    chaine = signature_to_str(sgn);
    printf("signaturetostr: %s \n", chaine);
    free(sgn->content);
    free(sgn);
    sgn = str_to_signature(chaine);
    printf("strtosignature: ");
    print_long_vector(sgn->content, sgn->size);

    // Testing protected:
    Protected *pr = init_protected(pKey, mess1, sgn);
    // Verification:
    if (verify(pr))
    {
        printf("Signaturevalide\n");
    }
    else
    {
        printf("Signature non valide \n");
    }
    chaine = protected_to_str(pr);
    printf("protectedtostr: %s \n", chaine);
    pr = str_to_protected(chaine);
    printf("strtoprotected: %s %s %s \n", key_to_str(pr->pKey), pr->mess,
           signature_to_str(pr->sgn));

    free(pKey);
    free(sKey);
    free(pKeyC);
    free(sKeyC);
    return 0;
}