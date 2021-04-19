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
    int prefix = 0;
    char* prefix_name = "";
    int i;
    for(i=1;i<argc;i++){
        if(argv[i][1] == 'p'){
            prefix = 1;
            prefix_name = argv[i]+2;
        }
    }
    int fd  = system_call(OPEN,".",O_RDONLY,DEFAULT);
    if(fd < 0)
        system_call(EXIT,COD,0,0);

    int fd_dents;
    linux_dirent* ent;
    char buf[SIZE];
    int d_type;
    char* type;
    while(1){
        fd_dents = system_call(GETDENTS,fd,buf,SIZE);
        if(fd_dents<0)
            system_call(EXIT,COD,0,0);
        if(fd_dents==0)
            break;
        for(i=0;i<fd_dents;i++){
            ent = (linux_dirent*)(buf+i);
            if(prefix){
                d_type = *(buf + i + ent->len - 1);
                type = (d_type == DT_REG ?  " regular" : (d_type == DT_DIR) ?  " directory" : (d_type == DT_FIFO) ? " FIFO" : (d_type == DT_SOCK) ? " socket" : (d_type == DT_LNK) ?  " symlink" : (d_type == DT_BLK) ?  " block dev" : (d_type == DT_CHR) ?  " char dev" : " ???");
                if(strncmp(prefix_name,ent->name,strlen(prefix_name))==0){
                    system_call(WRITE,STDOUT,"File name: ",11);
                    system_call(WRITE,STDOUT,ent->name,strlen(ent->name));
                    system_call(WRITE,STDOUT,", it's type:",12);
                    system_call(WRITE,STDOUT,type,strlen(type));
                    system_call(WRITE,STDOUT,"\n",1);
                }
            } else {
                system_call(WRITE,STDOUT,"File name: ",11);
                system_call(WRITE,STDOUT,ent->name,strlen(ent->name));
                system_call(WRITE,STDOUT,"\n",1);
            }
            i += ent->len - 1;
        }
    }
    return 0;
}