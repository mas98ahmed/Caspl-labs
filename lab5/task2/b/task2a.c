#include "LineParser.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <termios.h>
#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0

typedef struct process{
    cmdLine *cmd;         /* the parsed command line*/
    pid_t pid;            /* the process id that is running the command*/
    int status;           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next; /* next process in chain */
} process;

void execute(cmdLine *pCmdLine);
void addProcess(process **process_list, cmdLine *cmd, pid_t pid);
void removeProcess(process **list_Process, process *toRemove);
void printProcessList(process **process_list);
void freeProc(process *freeMe);
void updateProcessList(process **process_list);
void updateProcessStatus(process *process_list, int pid, int status);
void freeProcessList(process *process_list);

int debug = 0;
process* list;

int main(int argc, char **argv){
    cmdLine *pCmdLine;
    int i;
    for (i = 0; i < argc; i++){
        if (strcmp(argv[i], "-d") == 0){
            debug = 1;
        }
    }
    while (1){
        char buf[PATH_MAX];
        getcwd(buf, PATH_MAX);
        printf("%s>", buf);
        char input[2048];
        fgets(input, 2048, stdin);
        pCmdLine = parseCmdLines(input);
        if (strcmp(pCmdLine->arguments[0], "quit") == 0){
            freeProcessList(list);
            freeCmdLines(pCmdLine);
            break;
        }else if (strcmp(pCmdLine->arguments[0], "cd") == 0){
            if (chdir(pCmdLine->arguments[1]) == -1){
                perror("error :");
            }else{
                getcwd(buf, PATH_MAX);
            }
            freeCmdLines(pCmdLine);
        }else if (strcmp(pCmdLine->arguments[0], "procs") == 0){
            printProcessList(&list);
            freeCmdLines(pCmdLine);
        }else{
            execute(pCmdLine);
        }
    }
    return 0;
}


void execute(cmdLine *CmdLine)
{
    pid_t pid = fork();
    if (pid == 0){
        int ans = execvp(CmdLine->arguments[0], CmdLine->arguments);
        freeCmdLines(CmdLine);
        if (ans == -1){
            perror("error :");
            exit(1);
        }
    }else{
        addProcess(&list, CmdLine, pid);
        waitpid(pid, NULL, !(CmdLine->blocking));
    }
}


void addProcess(process **process_list, cmdLine *cmd, pid_t pid){
    process *p = malloc(sizeof(process));
    p->cmd = cmd;
    p->pid = pid;
    p->status = 1;
    if (*process_list == NULL){
        p->next = NULL;
        *process_list = p;
    }
    else{
        process *tmp = *process_list;
        p->next = tmp;
        *process_list = p;
    }
}

void printProcessList(process **process_list){
    updateProcessList(process_list);
    process *tmp = *process_list;
    printf("PID       Command       STATUS\n");
    while (tmp != NULL){
        printf("%d       %s       %s\n", tmp->pid, tmp->cmd->arguments[0],
            tmp->status == -1 ? "TERMINATED" : tmp->status == 0 ? "SUSPENDED" : "RUNNING");
        if (tmp->status == TERMINATED){
            removeProcess(process_list, tmp);
        }
        tmp = tmp->next;
    }
}

void removeProcess(process **list_Process, process *toRemove){
    process *list = *list_Process;
    if (list == toRemove){
        *list_Process = list->next;
        freeProc(toRemove);
    }else{
        while (list != NULL){
            if (list == toRemove){
                list->next = toRemove->next;
                freeProc(toRemove);
            }
            list = list->next;
        }
    }
}

void freeProc(process *freeMe){
    freeCmdLines(freeMe->cmd);
    free(freeMe);
}

void updateProcessList(process **process_list){
    process *p = *process_list;
    int status;
    while (p != NULL){
        int ret = waitpid(p->pid, &status, WNOHANG);
        if (ret != 0){
            p->status = ret;
        }
        p = p->next;
    }
}
void updateProcessStatus(process *process_list, int pid, int status){
    process *p = process_list;
    while (p != NULL){
        if (pid == p->pid){
            p->status = status;
        }
        p = p->next;
    }
}

void freeProcessList(process *process_list){
    while (process_list != NULL){
        process *p = process_list;
        process_list = process_list->next;
        freeProc(p);
    }
}