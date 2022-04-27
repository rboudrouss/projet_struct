#include "block.h"
#include "files.h"

#ifndef __UTILST__
#define __UTILST__

#define RED "\033[31m"
#define WHITE "\033[97m"
#define print_f(a) printf(RED a WHITE)

#define TFOLDER "tests"
#define BCFOLDER_T TFOLDER "/blockchain"
#define TMPFOLDER_T BCFOLDER_T "/temp"
#define KEYSF_T TMPFOLDER_T "/keys.txt"
#define CANDF_T TMPFOLDER_T "/candidates.txt"
#define DECF_T TMPFOLDER_T "/declarations.txt"
#define PENDV_T TMPFOLDER_T "/pending_votes.txt"
#define PENDB_T TMPFOLDER_T "/pending_block"

void print_long_vector(long *result, int size);
int longl_cmp(long* l1, long* l2);
CellKey* generate_cellkeys(int s);
Key* create_key(long val, long n);
CellProtected* generate_cellprotected(int s, char* msg);
Signature* dup_sign(Signature* s);
Key* dup_key(Key* k);

#endif