#include <openssl/sha.h>
#include <string.h>
#include "block.h"

void write_block(char *name)
{
    // TODO q7.1
}

Block *read_block(char *name)
{
    // TODO q7.2
}

char *block_to_str(Block *block)
{
    // FIXME hmm faudrait print toutes les clés aussi
    char *key = key_to_str(block->author);
    char *vote = protected_to_str(block->votes->data);
    char *rep = malloc(STR_SIZE * sizeof(char));
    sprintf(rep, "%s %s %s %d", key, block->previous_hash, vote, block->nonce);
    free(key);
    free(vote);
    return rep;
}

char *hash(char *s, int nonce)
{
    return SHA256(s, strlen(s), nonce);
}

// premier d zero en écriture hexa
int first_d_zero(char *s, int d)
{
    if (d == 1)
    {
        return s[0] >> 2 == 0;
    }

    for (int i = 0; i < d / 2; i++)
    {
        if (s[i] != 0)
            return 0;
    }

    if (d % 2 == 1)
        return s[((int)d / 2) + 1] >> 2 == 0;
    return 1;
}

void compute_proof_of_work(Block *b, int d)
{
    b->nonce = 0;
    char *strb = block_to_str(b);
    b->hash = SHA256(strb, strlen(strb), b->nonce);

    while (!first_d_zero(b->hash, d))
    {
        b->nonce++;
        free(b->hash);
        b->hash = SHA256(strb, strlen(strb), b->nonce);
    }
    free(strb);
}

int verify_block(Block *b, int d)
{
    char *strb = block_to_str(b);
    char *hash = SHA256(strb, strlen(strb), b->nonce);
    int rep = !strcmp(hash, b->hash);
    rep = rep && first_d_zero(b->hash, d);
    free(strb);
    free(hash);
    return rep;
}

// TODO q7.8

void delete_block(Block* b)
{
    free(b->hash);
    free(b->previous_hash);
    delete_only_list_protected(b->votes);
}

void delete_all_block(Block* b)
{
    free(b->hash);
    free(b->previous_hash);
    delete_list_protected(b->votes);
}