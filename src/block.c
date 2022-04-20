#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "block.h"

void write_block(char* filename, Block* block) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        printerror("Erreur lors de l'ouverture du fichier (write_block).\n"); return;
    }

    char* buffer = key_to_str(block->author);
    fprintf(f, "%s %s %s %d\n", buffer, block->hash, block->previous_hash, block->nonce);
    free(buffer);

    CellProtected* iter = block->votes;
    while (iter != NULL) {
        buffer = protected_to_str(iter->data);

        fprintf(f, "%s\n", buffer);
        free(buffer);

        iter = iter->next;
    }

    fclose(f);
}


Block* read_block(char* filename) {
    char buff[STR_SIZE];
    getcwd(buff, sizeof(buff));

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printerror("Erreur lors de l'ouverture du fichier (read_block).\n"); return NULL;
    }

    int nonce;
    char key_str[16], hash[128], prev_hash[128];   
    fscanf(f, "%s %s %s %d\n", key_str, hash, prev_hash, &nonce);

    Key* key = str_to_key(key_str);

    // Creating the block
    Block* block = (Block*) malloc(sizeof(Block));
    if (block == NULL) {
        printerror("Erreur d'allocation (read_block).\n");
        return NULL;
    }

    // Storing known values
    block->author = key;
    block->nonce = nonce;
    if (strcmp(hash, "(null)") == 0) block->hash = (unsigned char*) NULL;
    else block->hash = (unsigned char*) strdup(hash);

    if (strcmp(prev_hash, "(null)") == 0) block->previous_hash = (unsigned char*) NULL;
    else block->previous_hash = (unsigned char*) strdup(prev_hash);

    // Reading votes
    char pr_str[128], temp1[128], temp2[128];
    Protected* pr;
    CellProtected *cpr, *liste = NULL;
    while (!feof(f)) {
        fscanf(f, "%s %s %s\n", pr_str, temp1, temp2);
        strcat(strcat(strcat(strcat(pr_str, " "), temp1)," "), temp2);

        pr = str_to_protected(pr_str);
        cpr = create_cell_protected(pr);

        ajout_tete_cp(&liste, cpr);
    }

    fclose(f);

    block->votes = liste;

    return block;
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