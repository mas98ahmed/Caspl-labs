#include <dirent.h>
#define _GNU_SOURCE
#include "util.h"
#define WRITE 4
#define OPEN 5
#define DEFAULT 0640
#define CLOSE 6
#define STDERR 2
#define STDOUT 1
#define STDIN 0
#define READ 3
#define NULL 0
#define MAX_ 16
#define MAX 11
#define EXIT 1
#define COD 0x55
#define SIZE 8192
#define O_RDONLY 0
#define GETDENTS 141
extern int system_call();

enum
  {
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

       
typedef struct linux_dirent{
    int inode;
    int offset;
    short len;
    char name[];
}linux_dirent;

int main(int argc, char** argv) 
{ 
    linux_dirent *ent;  
    char buff[SIZE];
    int fd, fd1, fd2, i, debug, prefix, flag;
    debug = 0;
    prefix = 0;
    flag = 0;
    char d_type;
    char* fixName;
    char c[1] = "\n";
    

    fd = system_call(OPEN, ".", O_RDONLY, DEFAULT);
    
    if (fd < 0)         
        system_call(EXIT, COD, 0, 0);
    while(1) {
        fd1 = system_call(GETDENTS, fd, buff, SIZE);
        
        if (fd1 <= -1)
            system_call(EXIT, COD, 0, 0);
        if (fd1 == 0)
            break;
        for (i = 0; i < fd1;) {
            ent = (linux_dirent*)(buff + i);
            d_type = *(buff + i + ent->len - 1);
            if(!prefix){
                flag = 1;
                fd2 = system_call(WRITE,STDOUT,ent->name,strlen(ent->name));
                system_call(WRITE,STDOUT,(d_type == DT_REG ?  "  regular" : (d_type == DT_DIR) ?  "  directory" : (d_type == DT_FIFO) ? "  FIFO" : (d_type == DT_SOCK) ? "  socket" : (d_type == DT_LNK) ?  "  symlink" : (d_type == DT_BLK) ?  "  block dev" : (d_type == DT_CHR) ?  "  char dev" : "  ???"),11);
            }
            if(flag)
                system_call(WRITE,STDOUT,c,1);
            i = i + ent->len;
            flag = 0;
        }
    } 
    return 0; 
} 