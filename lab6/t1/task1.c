#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

#define stdin 0
#define stdout 1

int main(int argc, char **argv)
{
    int i;
    int debug = 0;
    for (i = 0; i < argc; i++){
        if (strcmp(argv[i], "-d") == 0){
            debug = 1;
        }
    }
    int fd[2];
    pipe(fd);
    if (debug == 1){
        printf("parent_process>forking…\n");
    }
    int child1 = fork();
    if (debug == 1){
        printf("parent_process>created process with id: %d\n", child1);
    }
    if (child1 == 0){
        close(stdout);
        dup(fd[1]);
        printf("parent_process>closing the write end of the pipe…");
        close(fd[1]);
        char *command1[] = {"ls", "-l", 0};
        if (execvp(command1[0], command1) == -1)
        {
            perror("error: ");
            exit(0);
        }
    }

    if (child1 > 0){
        close(fd[1]);
        int child2 = fork();
        if (child2 == 0){
            close(stdin);
            printf("parent_process>closing the read end of the pipe…");
            dup(fd[0]);
            close(fd[0]);
            char *command2[] = {"tail", "-n", "2", 0};
            if (execvp(command2[0], command2) == -1){
                perror("error:");
                exit(0);
            }
        }
        if (child2 > 0){
            printf("parent_process>waiting for child processes to terminate…\n");
            close(fd[0]);
            waitpid(child1, NULL, WUNTRACED);
            waitpid(child2, NULL, WUNTRACED);
            printf("parent_process>exiting…\n");
        }
    }
    return 0;
}
