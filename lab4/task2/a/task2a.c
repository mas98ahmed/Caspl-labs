#include <dirent.h>
#define _GNU_SOURCE
#include "util.h"

#define EXIT 1
#define READ 3
#define WRITE 4
#define OPEN 5
#define CLOSE 6
#define GETDENTS 141
#define DEFAULT 0644
#define SIZE 8192

#define STDERR 2
#define STDOUT 1
#define STDIN 0

#define O_RDONLY 0
#define O_CREAT 64

#define COD 0x55

extern int system_call();

typedef struct linux_dirent{
    int inode;
    int offset;
    short len;
    char name[];
} linux_dirent;

enum{
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4,
# define DT_DIR		DT_DIR
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8,
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
};

int main(int argc, char* argv[]){
    int debug = 0;
    int call_value; 
    int i;
    for(i=1;i<argc;i++){
        if(argv[i][1] == 'D'){
            debug = 1;
        }
    }
    int fd  = system_call(OPEN,".",O_RDONLY,DEFAULT);
    if(debug){
        system_call(WRITE,STDERR," Call id: ",11);
        system_call(WRITE,STDERR,itoa(OPEN),1);
        system_call(WRITE,STDERR,", Return value: ",16);
        system_call(WRITE,STDERR,itoa(fd),1);
        system_call(WRITE,STDOUT,"\n",1);
    }
    if(fd < 0)
        system_call(EXIT,COD,0,0);

    int fd_dents;
    linux_dirent* ent;
    char buf[SIZE];
    while(1){
        fd_dents = system_call(GETDENTS,fd,buf,SIZE);
        if(debug){
            system_call(WRITE,STDERR," Call id: ",11);
            system_call(WRITE,STDERR,itoa(GETDENTS),strlen(itoa(GETDENTS)));
            system_call(WRITE,STDERR,", Return value: ",16);
            system_call(WRITE,STDERR,itoa(fd_dents),strlen(itoa(fd_dents)));
            system_call(WRITE,STDOUT,"\n",1);
        }
        if(fd_dents<0)
            system_call(EXIT,COD,0,0);
        if(fd_dents==0)
            break;
        for(i=0;i<fd_dents;i++){
            ent = (linux_dirent*)(buf+i);
            system_call(WRITE,STDOUT,"File name: ",11);
            call_value = system_call(WRITE,STDOUT,ent->name,strlen(ent->name));
            system_call(WRITE,STDOUT,"\n",1);
            if(debug){
                system_call(WRITE,STDERR," Call id: ",11);
                system_call(WRITE,STDERR,itoa(WRITE),1);
                system_call(WRITE,STDERR,", Return value: ",16);
                system_call(WRITE,STDERR,itoa(call_value),1);
                system_call(WRITE,STDOUT,"\n",1);
            }
            i += ent->len - 1;
        }
    }
    return 0;
}