#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rsa.h"
#include "protec.h"

Protected *init_protected(Key *pKey, char *mess, Signature *sgn)
{
    Protected *rep = malloc(sizeof(Protected));
    rep->mess = strdup(mess);
    rep->sgn = sgn;
    rep->pKey = pKey;
    return rep;
}

void free_protected(Protected *p)
{
    free_signature(p->sgn);
    free_key(p->pKey);
    free(p->mess);
    free(p);
}

int verify(Protected *pr)
{
    if (strlen(pr->mess) != pr->sgn->size)
        return 0; // make it faster, it takes some time to decrypt
    char *decr = decrypt(pr->sgn->content, pr->sgn->size, pr->pKey->val, pr->pKey->n);
    int rep = strcmp(decr, pr->mess);
    free(decr);
    return !rep;
}

char *protected_to_str(Protected *pr)
{
    char *rep = malloc(STR_SIZE * sizeof(char));
    sprintf(rep, "%s %s %s", key_to_str(pr->pKey), pr->mess, signature_to_str(pr->sgn));
    return rep;
}

Protected *str_to_protected(char *str)
{
    char key[STR_SIZE], sign[STR_SIZE], mess[STR_SIZE];
    sscanf(str, "%s %s %s", key, mess, sign);
    return init_protected(
        str_to_key(key),
        mess,
        str_to_signature(sign));
}