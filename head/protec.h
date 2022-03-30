#ifndef __PRT__
#define __PRT__

#include "keys.h"
#include "signs.h"

typedef struct _protec
{
    Key* pKey;
    char* mess;
    Signature* sgn;
} Protected;

Protected *init_protected(Key *pKey, char *mess, Signature *sgn);
int verify(Protected *pr);
char *protected_to_str(Protected *pr);
Protected *str_to_protected(char *str);


#endif