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
    t->data = svec_copy(sv);

    int ind = svec_ind_of(sv, ";");
    if(ind > -1) {//make tree around ind
	t->op[0] = ';';
	t->op[1] = 0;
	t->left = make_tree(svec_copy_partial_to(sv, ind));
	t->right = make_tree(svec_copy_partial_from(sv, ind+1));
	return t;
    }

    ind = svec_ind_of(sv, ">");
    if(ind > -1) {//make tree around ind
	t->op[0] = '>';
	t->op[1] = 0;
	t->left = make_tree(svec_copy_partial_to(sv, ind));
	t->right = make_tree(svec_copy_partial_from(sv, ind+1));
	return t;
    }

    ind = svec_ind_of(sv, "<");
    if(ind > -1) {//make tree around ind
	t->op[0] = '>';
	t->op[1] = 0;
	t->left = make_tree(svec_copy_partial_to(sv, ind));
	t->right = make_tree(svec_copy_partial_from(sv, ind+1));
	return t;
    }

    //no operators
    t->op[0] = '=';
    t->op[1] = 0;
    if(t->op[0] != '=') {
        t->left = malloc(sizeof(tree));
        t->right = malloc(sizeof(tree));
    } else {
	t->left = NULL;
	t->right = NULL;
    }
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
	    //svec_print(t->data);
	    free_svec(t->data);
    }
    free(t);
}

