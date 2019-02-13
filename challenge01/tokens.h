#ifndef TOKENS
#define TOKENS

#include "svec.h"

int equal_token(char c);
int equal_double_token(char c1, char c2);
svec* tokenize(char* line);

#endif
