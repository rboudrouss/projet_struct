#include "keys.h"
#include "protec.h"

#ifndef __CELLS__
#define __CELLS__

typedef struct cellKey
{
    Key* data;
    struct cellKey* next;
} CellKey;

typedef struct cellProtected
{
    Protected* data;
    struct cellProtected* next;
} CellProtected;


#endif