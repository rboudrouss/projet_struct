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