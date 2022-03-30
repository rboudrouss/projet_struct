#include "keys.h"

#ifndef __SIGN__
#define __SIGN__

typedef struct _sign
{
    long* content;
    int size;
} Signature;

Signature *init_signature(long *content, int size);
void free_signature(Signature* sign);
Signature *sign(char *mess, Key *sKey);
char *signature_to_str(Signature *sgn);
Signature *str_to_signature(char *str);

#endif