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
    if (!p)
        return;
    free_signature(p->sgn);
    free_key(p->pKey);
    free(p->mess);
    free(p);
}

int verify(Protected *pr)
{
    if (!pr)
        return 0;
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
    char *k = key_to_str(pr->pKey), *s = signature_to_str(pr->sgn);
    sprintf(rep, "%s %s %s", k, pr->mess, s);
    free(k);
    free(s);
    return rep;
}

Protected *str_to_protected(char *str)
{
    if (!str)
        return NULL;
    char key[STR_SIZE], sign[STR_SIZE], mess[STR_SIZE];
    sscanf(str, "%s %s %s", key, mess, sign);
    return init_protected(
        str_to_key(key),
        mess,
        str_to_signature(sign));
}

void submit_vote(Protected *p)
{
    FILE *f;
    f = fopen(PENDV, "a+");
    if (!f){
        printf("submit_vote : file does not exist, opening in read mode");
        f = fopen(PENDV, "w");
    }
    char *vote = protected_to_str(p);
    fputs(vote, f);
    fputc('\n', f);
    free(vote);
    fclose(f);
}