#ifndef TREE
#define TREE
#include <stdlib.h>
#include "svec.h"

typedef struct tree tree;
struct tree {
    char* op;//= will represent no operator
    tree* left;
    tree* right;
    svec* data;
};

tree* make_tree(svec* sv);
void  free_tree(tree* t);
#endif
