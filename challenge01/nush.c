#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "tokens.h"
#include "svec.h"
#include "tree.h"

int
streq(char* str1, char* str2) {
	return strcmp(str1, str2) == 0;
}
/*
 * Plan:
 * with example: echo one two; echo three
 * step 1: tokenize into list
 * [echo, one, two, ;, echo, three]
 * step 2: assemble tree
 * Make leaves single command (list of tokens), parent is symbol
 * [echo, one, two] / ; \ [echo, three]
 * step 3: manage symbol
 * step 4: recur on children
 */
void
execute(tree* t)
{
    if(streq(t->op, "=")) {
	if(t->data->size == 0) {
	    return;
        }
        if(streq(t->op, "=") && streq(svec_get(t->data, 0), "exit")) {
	    exit(0);
        }
        if(streq(t->op, "=") && streq(svec_get(t->data, 0), "cd")) {
	    chdir(svec_get(t->data, 1));
	    return;
        }
    }
    
    if(streq(t->op, ";")) {
        execute(t->left);
	execute(t->right);
	return;
    }

    if(streq(t->op, ">")) {
	int cpid;
	if((cpid = fork())) {
	    //parent
	    int status;
	    waitpid(cpid, &status, 0);
	} else {
	    //child
	    if(t->right == NULL) {
		    printf("Right of tree should not be null");
		    exit(1);
	    }
	    svec* cmd = t->right->data;
	    if(cmd->size == 0) {
		printf("You must redirect a file");
		exit(1);
	    }
	    int fd = open(svec_get(cmd, 0), O_WRONLY | O_CREAT, 0666);
	    
	    //make output the file
	    close(1);
	    dup(fd);
	    close(fd);

	    execute(t->left);
	    exit(0);
	}	
	return;
    }

    if(streq(t->op, "<")) {
	return;
    }

    int cpid;

    if ((cpid = fork())) {
        // parent process
        int status;
        waitpid(cpid, &status, 0);
    }
    else {
        // child process
	if(streq(t->op, "=")) {
	    svec* cmd = t->data;
	    if (cmd->size == 0) {
	        exit(0);
	    }
            char* args[cmd->size + 1];
	    for(int i = 0; i < cmd->size; ++i) {
	       	    args[i] = svec_get(cmd, i);
	    }
	    args[cmd->size] = 0;

            execvp(svec_get(cmd, 0), args);
            printf("Can't get here, exec only returns on error.");
	}
    }
}

int
main(int argc, char* argv[])
{
    char input[256];
    int offset = 0;
    if (argc != 1) {
	int fd = open(argv[1], O_RDONLY);
	read(fd, input, 256);
	close(fd);
    }
    while(1) {
    	char cmd[256];

    	if (argc == 1) {
        	printf("nush$ ");
        	fflush(stdout);
        	char* rv = fgets(cmd, 256, stdin);
		if (!rv) {
		    exit(0);
		}
    	}
    	else {
		for (int i = 0; i < strlen(input)-offset+1; ++i) {
		   if(input[i+offset] == '\n' || input[i+offset] == 0) {
	 	       cmd[i] = 0;
		       offset += i+1;
		       break;
		   }
		   cmd[i] = input[offset+i];
		}
		if(strlen(cmd) == 0) {
		    exit(0);
		}
    	}

    	svec* tokens = tokenize(cmd);
   	 //convert to tree
   	 tree* t = make_tree(tokens);
   	 execute(t);//take in tree
   	 free_tree(t);

   	 free_svec(tokens);
    }
    return 0;
}
