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

//I added this
svec* svec_copy(svec* sv) {
   if (sv == NULL) {
      return NULL;
   }
   svec* rv = make_svec();
   for(int i=0; i < sv->size; i++) {
	svec_push_back(rv, svec_get(sv, i));
   }
   return rv;
}

//I added this
svec* svec_copy_partial_to(svec* sv, int ub) {
   if (sv == NULL) {
      return NULL;
   }
   if (ub > sv->size) {
	printf("You should not be copying more of the svec than exists...");
	return svec_copy(sv);
   }
   svec* rv = make_svec();
   for(int i=0; i < ub; i++) {
	svec_push_back(rv, svec_get(sv, i));
   }
   return rv;
}

//I added this
svec* svec_copy_partial_from(svec* sv, int lb) {
   if (sv == NULL) {
      return NULL;
   }
   if (lb > sv->size) {
	printf("You should not be copying past the end of svec...");
	return svec_copy(sv);
   }
   svec* rv = make_svec();
   for(int i=lb; i < sv->size; i++) {
	svec_push_back(rv, svec_get(sv, i));
   }
   return rv;
}

//I added this
int svec_ind_of(svec* sv, char* find) {
   if (sv == NULL) {
      return -1;
   }
   for(int i=0; i < sv->size; i++) {
	if (strcmp(svec_get(sv, i), find) == 0) {
		return i;
	}
   }
   return -1;
}
