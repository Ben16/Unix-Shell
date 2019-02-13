/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */
//as this says, I got this from lecture notes

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "svec.h"

svec*
make_svec()
{
    svec* sv = malloc(sizeof(svec));
    sv->size = 0;
    sv->cap  = 4;
    sv->data = malloc(4 * sizeof(char*));
    memset(sv->data, 0, 4 * sizeof(char*));
    return sv;
}

void
free_svec(svec* sv)
{
    for (int ii = 0; ii < sv->size; ++ii) {
        if (sv->data[ii] != 0) {
            free(sv->data[ii]);
        }
    }
    free(sv->data);
    free(sv);
}

char*
svec_get(svec* sv, int ii)
{
    assert(ii >= 0 && ii < sv->size);
    return sv->data[ii];
}

void
svec_put(svec* sv, int ii, char* item)
{
    assert(ii >= 0 && ii < sv->size);
    sv->data[ii] = strdup(item);

}

void svec_push_back(svec* sv, char* item)
{
    int ii = sv->size;

    if (ii >= sv->cap) {
        sv->cap *= 2;
        sv->data = (char**) realloc(sv->data, sv->cap * sizeof(char*));
    }

    sv->size = ii + 1;
    svec_put(sv, ii, item);
}

//I added this
void svec_print(svec* sv)
{
    for(int i = 0; i < sv->size; ++i) {
        printf("%s\n", svec_get(sv, i));
    }
}

//I added this
void svec_reverse(svec* sv)
{
    char ** data = sv->data;
    for(int i = 0; i < sv->size/2; ++i) {
        char* tmp = data[i];
        data[i] = data[sv->size - i - 1];
	data[sv->size - i - 1] = tmp;
    }
}
