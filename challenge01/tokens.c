#include <stdlib.h>
#include <stdio.h>

#include "svec.h"

int
equal_token(char c) {
 return (c == '|') || (c == '&') || (c == '<') || (c == '>') || (c == ';');
}

int
equal_double_token(char c1, char c2) {
 return (c1 == '|' && c2 == '|') || (c1 == '&' && c2 == '&');
}

svec*
tokenize(char* line) {
    svec* tokens = make_svec();
    int i = 0;
    while(line[i] != 0) {
        if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t') {
	    //some form of white space, keep going
	    ++i;
	    continue;
	}
	if (equal_token(line[i])) {
	    if(line[i+1] != 0 && equal_double_token(line[i], line[i+1])) {
                //add double token
		char* tok = malloc(3);
		tok[0] = line[i];
		tok[1] = line[i+1];
		tok[2] = 0;
	        svec_push_back(tokens, tok);
		free(tok);
		i += 2;
	    }
	    else {
                //add single token
                char* tok = malloc(2);
		tok[0] = line[i];
		tok[1] = 0;
	        svec_push_back(tokens, tok);
		free(tok);
		i += 1;
	    }
	}
	else {
            //add word
	    int j = i+1;
	    while (line[j] != 0 && line[j] != ' ' && line[j] != '\n'  && line[i] != '\t' && !(equal_token(line[j]))) {
                ++j;
	    }
	    char* tok = malloc(j-i);
	    for (int k = 0; k < j-i; ++k) {
                tok[k] = line[i+k];
	    }
	    tok[j-i] = 0;
	    svec_push_back(tokens, tok);
	    free(tok);
	    i = j;
	}
    }
    return tokens;
}
/* TODO:

   while (1) {
     printf("tokens$ ");
     fflush(stdout);
     line = read_line()
     if (that was EOF) {
        exit(0);
     }
     tokens = tokenize(line);
     foreach token in reverse(tokens):
       puts(token)
   }

*/
int
main(int _argc, char* _argv[]) {
   //reading in lines heavily adapted from lecture code
   char line[100];
   while(1) {
      printf("tokens$ ");
      fflush(stdout);
      char * e = fgets(line, 96, stdin);
      if(!e) {
         exit(0);
      }
      svec* tokens = tokenize(line);
      svec_reverse(tokens);
      svec_print(tokens);
      free_svec(tokens);
   }
}



