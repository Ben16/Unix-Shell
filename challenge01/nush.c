#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tokens.h"
#include "svec.h"
#include "tree.h"

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
execute(char* cmd)
{
    svec* tokens = tokenize(cmd);
    int cpid;

    if ((cpid = fork())) {
        // parent process
        printf("Parent pid: %d\n", getpid());
        printf("Parent knows child pid: %d\n", cpid);

        // Child may still be running until we wait.

        int status;
        waitpid(cpid, &status, 0);

        printf("== executed program complete ==\n");

        printf("child returned with wait code %d\n", status);
        if (WIFEXITED(status)) {
            printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
        }
    }
    else {
        // child process
        printf("Child pid: %d\n", getpid());
        printf("Child knows parent pid: %d\n", getppid());

        for (int ii = 0; ii < strlen(cmd); ++ii) {
            if (cmd[ii] == ' ') {
                cmd[ii] = 0;
                break;
            }
        }

        // The argv array for the child.
        // Terminated by a null pointer.
        char* args[] = {cmd, "one", 0};

        printf("== executed program's output: ==\n");

        execvp(cmd, args);
        printf("Can't get here, exec only returns on error.");
    }
}

int
main(int argc, char* argv[])
{
    char cmd[256];

    if (argc == 1) {
        printf("nush$ ");
        fflush(stdout);
        fgets(cmd, 256, stdin);
    }
    else {
        memcpy(cmd, "echo", 5);
    }

    svec* tokens = tokenize(cmd);

    //convert to tree
    tree* t = make_tree(tokens);
    //execute(cmd);//take in tree
    free_tree(t);

    free_svec(tokens);

    return 0;
}
