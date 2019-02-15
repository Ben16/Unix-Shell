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


    int cpid;

    if ((cpid = fork())) {
        // parent process
        //printf("Parent pid: %d\n", getpid());
        //printf("Parent knows child pid: %d\n", cpid);

        // Child may still be running until we wait.

        int status;
        waitpid(cpid, &status, 0);

        //printf("== executed program complete ==\n");

        //printf("child returned with wait code %d\n", status);
        /*if (WIFEXITED(status)) {
            printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
        }*/
    }
    else {
        // child process
        //printf("Child pid: %d\n", getpid());
        //printf("Child knows parent pid: %d\n", getppid());
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

            //printf("== executed program's output: ==\n");
            execvp(svec_get(cmd, 0), args);
            printf("Can't get here, exec only returns on error.");
	}
    }
}

int
main(int argc, char* argv[])
{
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
		/*fflush(stdout);
		int argcounter = 1;
		char* arg = argv[argcounter];
		int j = 0;
		cmd[255] = 0;
		for(int i = 0; i < 255; ++i) {
	 	    if(arg[j] == 0) {
			j = 0;
			++argcounter;
			if(argcounter >= argc) {
			    cmd[i] = 0;
			    break;
			}
			arg = argv[argcounter];
			cmd[i] = ' ';
			continue;
		    }
		    cmd[i] = arg[j];
		    ++j;
		}*/
		int fd = open(argv[1], O_RDONLY);
		int count = read(fd, cmd, 256);
		close(fd);
		//printf("%s\n", cmd);
		for (int i = 0; i < strlen(cmd); ++i) {
		   //properly null terminate
		   if(cmd[i] == '\n') {
	 	       cmd[i] = 0;
		   }
		}
    		/*char* current_num = malloc(4);
    		for (int i=0; ; i++) {
      		    long count = read(0, current_num+i, 1);
       		    if (count == 0 || current_num[i] == '\n') {
	   		current_num[i] = 0;
	   		long num = atol(current_num);
	   		push_vec(vv, num);
           		break;
       		    }
       		    if (current_num[i] == ' ') {
           		++space_count;
	   		//change current_num to long and add to vec
	   		long num = atol(current_num);
	   		push_vec(vv, num);
	   		for (int j=0; j < 4; j++) {
               			current_num[j]=0;
	   		}
	   		i = -1;
       		    }
    		}*/
	
    	}

    	svec* tokens = tokenize(cmd);
   	 //convert to tree
   	 tree* t = make_tree(tokens);
   	 execute(t);//take in tree
   	 free_tree(t);

   	 free_svec(tokens);
	 if(argc != 1) {
	     exit(0);
	 }
    }
    return 0;
}
