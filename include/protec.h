#include "signs.h"
#include "files.h"

#ifndef __PRT__
#define __PRT__

typedef struct _protec
{
    Key *pKey;
    char *mess;
    Signature *sgn;
} Protected;

Protected *init_protected(Key *pKey, char *mess, Signature *sgn);
void free_protected(Protected *p);
int verify(Protected *pr);
char *protected_to_str(Protected *pr);
Protected *str_to_protected(char *str);
void submit_vote(Protected* p);

#endif