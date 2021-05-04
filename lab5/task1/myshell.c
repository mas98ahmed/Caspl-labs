#include "LineParser.c" 
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SIZE 2048

void execute(cmdLine *pCmdLine);

int debug ;

int main (int argc , char* argv[]){
    cmdLine* data ;
    char path[PATH_MAX];
    char buf[MAX_SIZE];

    if(argc>1){
        debug = (strcmp(argv[1],"-d")==0) ? 1 : 0 ;
    }

    //printing the path.
    getcwd(path,PATH_MAX);
    printf("Directory path : %s\n" ,path);
    fgets(buf,MAX_SIZE ,stdin);

    //entering input and executing it.
    while(1){    
        if(strncmp(buf,"quit",4)==0)
            break;
        data = parseCmdLines(buf);
        execute(data);
        freeCmdLines(data);
        fgets(buf,MAX_SIZE ,stdin);   
    }
    return 0 ;
}

void execute(cmdLine* pCmdLine){
    if(strcmp(pCmdLine->arguments[0],"cd")==0){
            char* lib =pCmdLine->arguments[1];
        if(chdir(lib)!= 0){
            fprintf(stderr,"cd operation fails\n");
        }

    }else{
        int pid , status ;
        if((pid=fork())==0){
            execvp( pCmdLine->arguments[0], pCmdLine->arguments);
            perror("error :");
            exit(0);
        }
        if(debug){ fprintf(stderr ,"command is %s and pid = %d\n",pCmdLine->arguments[0],pid); }
        waitpid(pid,&status,!(pCmdLine->blocking));
    }
    
}