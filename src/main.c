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
    CellKey *v = read_public_keys(KEYSF);
    // print_list_keys(v);
    CellKey *c = read_public_keys(CANDF);
    // print_list_keys(c);
    CellProtected *p = read_protected(DECF);
    // print_list_protected(p);
    Key *w = compute_winner(p, c, v, NC, NV);
    char *s = key_to_str(w);
    printf("Le gagnant de cette élection est %s\n", s);

    CellTree *tree = init_father_tree(p, NBD);
    write_block(BCFOLDER "/1.blk", tree->block);
    create_block(tree, w, NBD);
    add_block(NBD, BCFOLDER "/2.blk");

    print_tree(tree);
    // CellTree *tree2 = read_tree();
    // print_tree(tree2);

    return 0;
}
