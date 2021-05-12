#include "LineParser.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <sys/types.h>

#define STDIN 0
#define STDOUT 1
#define HistoryLength 10

void execute(cmdLine *cmd);
void printHistory();

char* history[HistoryLength];
int counter = 0;

int main(int argc, char **argv){
    int debug = 0;
    char buf[PATH_MAX];
    char input[2048];
    int terminate = 0;
    cmdLine *cmd;


    int i;
    for (i = 0; i < argc; i++){
        if (strcmp(argv[i], "-d") == 0){
            debug = 1;
        }
    }


    while (!terminate){
        getcwd(buf, PATH_MAX);
        printf("%s>", buf);
        fgets(input, 2048, stdin);
        history[counter] = input;
        counter+=1;
        cmd = parseCmdLines(input);

        if (strcmp(cmd->arguments[0], "quit") == 0){
            freeCmdLines(cmd);
            break;
        }else if (strcmp(cmd->arguments[0], "history") == 0){
            printHistory();
            freeCmdLines(cmd);
            break;
        }else if (strcmp(cmd->arguments[0], "cd") == 0){
            if (chdir(cmd->arguments[1]) == -1){
                freeCmdLines(cmd);
                perror("error:");
            }
            else{
                getcwd(buf, PATH_MAX);
                freeCmdLines(cmd);
                continue;
            }
        }
        if (cmd->next != NULL){
            int fd[2];
            pipe(fd);
            FILE *input;
            int pid = fork();
            if (pid == 0){
                close(STDOUT);
                dup(fd[1]);
                close(fd[1]);
                if (cmd->inputRedirect != NULL){
                    close(0);
                    input = fopen(cmd->inputRedirect, "r");
                    if (input == NULL)
                        perror("error:");
                }
                if (execvp(cmd->arguments[0], cmd->arguments) == -1){
                    perror("error:");
                    exit(0);
                }
                if (input)
                    fclose(input);
            }
            if (pid > 0){
                cmd = cmd->next;
                close(fd[1]);
                int pid2 = fork();

                if (pid2 == 0){
                    FILE *output = NULL;
                    close(STDIN);
                    dup(fd[0]);
                    close(fd[0]);
                    if (cmd->outputRedirect != NULL){
                        close(1);
                        output = fopen(cmd->outputRedirect, "a");
                        if (output == NULL)
                            perror("error:");
                    }
                    if (execvp(cmd->arguments[0], cmd->arguments) == -1){
                        perror("error:");
                        exit(1);
                    }
                    if (output)
                        fclose(output);
                }
                if (pid2 > 0){
                    close(fd[0]);
                    waitpid(pid, NULL, WUNTRACED);
                    waitpid(pid2, NULL, WUNTRACED);
                }
            }
        }else{
            execute(cmd);
        }
        freeCmdLines(cmd);
    }
    return 0;
}

void execute(cmdLine *cmd){
    int fd[2];
    int pid = fork();
    pipe(fd);

    if (pid == 0){
        FILE *in, *out;
        if (cmd->inputRedirect != NULL){
            close(STDIN);
            in = fopen(cmd->inputRedirect, "r");
            if (in == NULL)
                perror("error:");
        }
        if (cmd->outputRedirect != NULL){
            close(STDOUT);
            out = fopen(cmd->outputRedirect, "a");
            if (out == NULL)
                perror("error:");
        }
        if (execvp(cmd->arguments[0], cmd->arguments) == -1){
            perror("error:");
            exit(1);
        }

        if (in)
            fclose(in);
        if (out)
            fclose(out);
    }else{
        if (waitpid(pid, NULL, !cmd->blocking) == -1)
            perror("error:");
    }
}


void printHistory(){
    for(int i=0;i<counter;i++){
        printf("%s\n",history[counter]);
    }
}