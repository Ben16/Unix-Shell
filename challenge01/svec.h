/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */
//as this says, I took this from lecture notes

#ifndef SVEC_H
#define SVEC_H

typedef struct svec {
    int size;
    int cap;
    char** data;
} svec;

svec* make_svec();
void  free_svec(svec* sv);

char* svec_get(svec* sv, int ii);
void  svec_put(svec* sv, int ii, char* item);

void svec_push_back(svec* sv, char* item);

//void svec_sort(svec* sv);
void svec_print(svec* sv);
void svec_reverse(svec* sv);
svec* svec_copy(svec* sv);
svec* svec_copy_partial_to(svec* sv, int ub);
svec* svec_copy_partial_from(svec* sv, int lb);
int svec_ind_of(svec* sv, char* find);

#endif
