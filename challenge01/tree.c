#include "tree.h"
#include "svec.h"

/*typedef struct tree {
    char* op;//= will represent no operator
    tree* left;
    tree* right;
    svec* data;
} tree;*/

tree* 
make_tree(svec* sv) {
    tree* t = malloc(sizeof(tree));
    t->op = malloc(3*sizeof(char));
    t->left = malloc(sizeof(tree));
    t->right = malloc(sizeof(tree));
    t->data = svec_copy(sv);

    //for now
    t->op[0] = '=';
    t->op[1] = 0;
    return t;
}

void 
free_tree(tree* t) {
    if(t->op[0] != '=') {
	free_tree(t->left);
        free_tree(t->right);
    }
    free(t->op);
    if (t->data != NULL) {
	    svec_print(t->data);
	    free_svec(t->data);
    }
    free(t);
}

