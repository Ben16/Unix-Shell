#include <string.h>
#include <stdio.h>

#include "tree.h"
#include "svec.h"

void
check_tok(tree* t, char* sym) {
    if (strlen(sym) > 2) {
	printf("check_tok should only be used on one or two char shell tokens");
    }
    int ind;
    ind = svec_ind_of(t->data, sym);
    if(ind > -1) {//make tree around ind
	t->op[0] = sym[0];
	t->op[1] = sym[1];//in a one-char string, this will be null terminator
	t->op[2] = 0;
	t->left = make_tree(svec_copy_partial_to(t->data, ind));
	t->right = make_tree(svec_copy_partial_from(t->data, ind+1));
    }
}
tree* 
make_tree(svec* sv) {
    tree* t = malloc(sizeof(tree));
    t->op = malloc(3*sizeof(char));
    t->data = svec_copy(sv);

    //base case
    t->op[0] = '=';
    t->op[1] = 0;
    t->left = NULL;
    t->right = NULL;
    
    check_tok(t, ";");
    if (strcmp(t->op, ";") == 0) {
	return t;
    }
    check_tok(t, "<");
    if (strcmp(t->op, "<") == 0) {
	return t;
    }
    check_tok(t, ">");
    if (strcmp(t->op, ">") == 0) {
	return t;
    }
    check_tok(t, "&&");
    if (strcmp(t->op, "&&") == 0) {
	return t;
    }
    check_tok(t, "||");
    if (strcmp(t->op, "||") == 0) {
	return t;
    }
    check_tok(t, "|");
    if (strcmp(t->op, "|") == 0) {
	return t;
    }
    check_tok(t, "&");
    if (strcmp(t->op, "&") == 0) {
	return t;
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

