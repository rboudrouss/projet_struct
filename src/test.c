#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "protec.h"
void print_long_vector(long *result, int size)
{
    printf(" Vector : [ ");
    for (int i = 0; i < size; i++)
    {
        printf("%ld \t ", result[i]);
    }
    printf("] \n");
}

int main(void)
{

    srand(time(NULL));

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
    char *mess = key_to_str(pKeyC);
    printf("%s vote pour %s \n", key_to_str(pKey), mess);

    Signature *sgn = sign(mess, sKey);
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
    Protected *pr = init_protected(pKey, mess, sgn);
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