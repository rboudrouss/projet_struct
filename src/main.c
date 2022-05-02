#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "prime.h"
#include "rsa.h"
#include "generate.h"
#include "btree.h"

#define NC 20
#define NV 100
#define NP 10
#define NBD 2

#define NB_BLK 10

int main()
{
    srand(time(NULL));

    system("mkdir -p blockchain/temp");
    system("find blockchain -maxdepth 2 -type f -delete");

    printf("Generating random data... \n");
    fflush(stdout);
    generate_random_data(NV, NC);
    printf("done.\n");

    printf("Reading files... \n");
    fflush(stdout);
    CellProtected *votes = read_protected(DECF);
    CellKey *keys = read_public_keys_fromp(KEYSF);
    CellKey *candidates = read_public_keys(CANDF);
    printf("done.\n");

    CellProtected *votes_copy = votes;
    CellTree *tree = NULL;

    int count = 0, file_number = 0;
    char file_name[128];

    printf("Building tree... \n");
    fflush(stdout);

    Key *author = malloc(sizeof(Key));
    init_key(author, 0, 0);

    while (votes != NULL)
    {
        submit_vote(votes->data);

        count++;
        if (count == 10)
        {
            create_block(tree, author, NBD);

            file_number++;
            sprintf(file_name, "%s/%d.blk", BCFOLDER, file_number);
            add_block(NBD, file_name);

            count = 0;

            delete_tree(tree);

            tree = read_tree();
        }

        votes = votes->next;
    }

    // If some votes have not been block'd yet (if NBVOTERS % 10 != 0)
    if (count != 0)
    {
        create_block(tree, author, NBD);

        file_number++;
        sprintf(file_name, "%s/%d.blk", BCFOLDER, file_number);
        add_block(NBD, file_name);

        delete_tree(tree);

        tree = read_tree();
    }
    free(author);
    author = NULL;
    printf("done.\n");

    int should_print;
    printf("\nPrint tree ? (0: No, 1: Yes) ");
    scanf(" %d", &should_print);
    if (should_print)
        print_tree(tree);

    printf("\n");

    Key *winner = compute_winner_BT(tree, candidates, keys, NC, NV);

    if (winner != NULL)
        printf("Winner: (%lx, %lx)\n", winner->val, winner->n);

    // Freeing tree
    delete_tree(tree);

    // Freeing voters, candidates and votes
    delete_list_protected(votes_copy);
    delete_list_keys(keys);
    delete_list_keys(candidates);

    return 0;
}
