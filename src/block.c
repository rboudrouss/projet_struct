#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "block.h"

void write_block(char *filename, Block *block)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier (write_block).\n");
        fclose(f);
        return;
    }
    if (!block)
    {
        perror("no Block specified in 'write_block' function\n");
        fputc('\n', f);
        fclose(f);
        return;
    }

    char *buffer = key_to_str(block->author);
    fprintf(f, "%s %s %s %d\n", buffer, block->hash, block->previous_hash, block->nonce);
    free(buffer);

    reverse_cell_protected(&block->votes);

    CellProtected *iter = block->votes;
    while (iter != NULL)
    {
        buffer = protected_to_str(iter->data);

        fprintf(f, "%s\n", buffer);
        free(buffer);

        iter = iter->next;
    }

    // reverse_cell_protected(&block->votes); // TODO might not need

    fclose(f);
}

Block *read_block(char *filename)
{
    char buff[STR_SIZE];
    getcwd(buff, sizeof(buff));

    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier (read_block).\n");
        return NULL;
    }

    int nonce;
    char key_str[16], hash[128], prev_hash[128];
    fscanf(f, "%s %s %s %d\n", key_str, hash, prev_hash, &nonce);
    Key *key = str_to_key(key_str);

    // Creating the block
    Block *block = (Block *)malloc(sizeof(Block));
    if (block == NULL)
    {
        perror("Erreur d'allocation (read_block).\n");
        return NULL;
    }

    // Storing known values
    block->author = key;
    block->nonce = nonce;
    if (strcmp(hash, "(null)") == 0)
        block->hash = (unsigned char *)NULL;
    else
        block->hash = (unsigned char *)strdup(hash);

    if (strcmp(prev_hash, "(null)") == 0)
        block->previous_hash = (unsigned char *)NULL;
    else
        block->previous_hash = (unsigned char *)strdup(prev_hash);

    // Reading votes
    char pr_str[STR_SIZE], temp1[STR_SIZE], temp2[STR_SIZE];
    Protected *pr;
    CellProtected *liste = NULL;
    while (!feof(f))
    {
        fscanf(f, "%s %s %s\n", pr_str, temp1, temp2);

        char *temp12 = strdup(temp1); // HACK
        char *temp22 = strdup(temp2);
        char *full_str = strcat(strcat(strcat(strcat(pr_str, " "), temp12), " "), temp22);
        free(temp22);
        free(temp12);

        pr = str_to_protected(full_str);

        add_cell_protected(&liste, pr);
    }

    fclose(f);

    block->votes = liste;

    return block;
}

char *block_to_str(Block *block)
{
    int str_size = STR_SIZE;
    char *res = (char *)malloc(sizeof(char) * str_size);
    if (res == NULL)
    {
        perror("Erreur d'allocation (block_to_str).\n");
        return NULL;
    }

    // Writing author, previous hash, PoW
    char *buffer = key_to_str(block->author);
    sprintf(res, "%s %s %d\n", buffer, block->previous_hash, block->nonce);
    free(buffer);

    CellProtected *current = block->votes;
    // Writing all votes
    while (current != NULL)
    {
        buffer = protected_to_str(current->data);

        // Calculating the new string size and reallocating it
        str_size = str_size + strlen(buffer) + 2;
        res = realloc(res, str_size);

        strcat(strcat(res, buffer), " ");

        free(buffer);

        current = current->next;
    }

    return res;
}

unsigned char *hashf(char *s)
{
    unsigned char *hashed = SHA256((unsigned char *)s, strlen(s), 0);

    unsigned char *final = malloc(sizeof(unsigned char) * STR_SIZE);

    sprintf((char *) final, "%02x", hashed[0]);

    char buffer[8];
    for (int i = 1; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(buffer, "%02x", hashed[i]);
        strcat((char *) final, buffer);
    }
    return final;
}

// premier d zero en écriture hexa
int first_d_zero(unsigned char *s, int d)
{
    for (int i = 0; i < d; i++)
        if (s[i] != '0')
            return 0;
    return 1;
}

void compute_proof_of_work(Block *b, int d)
{
    b->nonce = 0;
    char *strb = block_to_str(b);
    if(b->hash)
        free(b->hash);
    b->hash = hashf(strb);

    while (!first_d_zero(b->hash, d))
    {
        b->nonce++;
        free(strb);
        free(b->hash);
        strb = block_to_str(b);
        b->hash = hashf(strb);
    }
    free(strb);
}

int verify_block(Block *b, int d)
{
    char *strb = block_to_str(b);
    unsigned char *hash = hashf(strb);
    int rep = !strcmp((char *)hash, (char *)b->hash);
    rep = rep && first_d_zero(b->hash, d);
    free(strb);
    free(hash);
    return rep;
}

void delete_block(Block *b)
{
    if (!b)
        return;
    free(b->hash);
    free(b->previous_hash);
    free_key(b->author);
    delete_list_protected(b->votes);
    free(b);
}

void delete_all_block(Block *b)
{
    free(b);
    free(b->hash);
    free(b->previous_hash);
    delete_only_list_protected(b->votes);
}