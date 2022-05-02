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

int main()
{
    srand(time(NULL));

    generate_random_data(NV, NC, NP);
    printf("Data generated\n");
    Key *author = malloc(sizeof(Key));
    init_key(author, 0, 0);

    CellProtected *p = read_protected(PENDV);

    delete_list_protected(p);
    free_key(author);

    // CellTree *tree = NULL;
    //  create_block(tree, author, NBD);
    //  add_block(NBD, BCFOLDER "/1.blk");

    // print_tree(tree);
    // CellTree *tree2 = read_tree();
    // print_tree(tree2);

    return 0;
}
