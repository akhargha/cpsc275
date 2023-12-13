/**
 * @author: Anupam Khargharia
 * Assn 5: CPSC 275
 * simple Linux shell program
 * Version: 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 1024 //max length of command lines
#define INITIAL_ARGV_SIZE 10 //initial size of arguments

int main() {
    char line[MAX_LINE];
    char **argv; //command arguments
    char *token; //individual arguments
    int argv_size = INITIAL_ARGV_SIZE; 

    //allocate initial memory
    argv = (char **)malloc(argv_size * sizeof(char*));

    while(1) {
        printf("mysh> ");

        if (!fgets(line, MAX_LINE, stdin)) { //read command from stdin
            break; // exit on EOF
        }

        // remove newline char and add terminator
        if ((strlen(line) > 0) && (line[strlen(line) - 1] == '\n')) {
            line[strlen(line) - 1] = '\0';
        }

        //quit command
        if (strcmp(line, "quit") == 0) {
            break;
        }

        int i = 0;
        token = strtok(line, " ");
        while (token != NULL) {
            if (i >= argv_size) { //check if more argv space is needed
                argv_size *= 2; // double the size
                argv = (char **)realloc(argv, argv_size * sizeof(char *));
            }
            // copy token into argv & allocate memory
            argv[i] = strdup(token);
            i++;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL; // terminate argv


        int pid = fork();
        if (pid == 0) {
            //child process
            if (execvp(argv[0], argv) < 0) {
                perror("execvp error");
                exit(EXIT_FAILURE);
            }
        }
        else {
            //wait for child to finish
            wait(NULL);
        }

        // free memory
        for (int j = 0; j < i; j++) {
            free(argv[j]);
        }
    }

    free(argv);
    return 0;
}
