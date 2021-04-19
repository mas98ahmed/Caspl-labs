#include "util.h"
#define WRITE 4
#define OPEN 5
#define DEFAULT 0640
#define CLOSE 6
#define STDERR 2
#define STDOUT 1
#define STDIN 0
#define READ 3
#define O_CREATE 64
#define O_WRONLY 1
#define O_RDONLY 0
#define EXIT 1
#define COD 0x55
extern int system_call();

int main(int argc, char** argv){
    int outFile = STDOUT;
    int inFile = STDIN;
    char* name1;
    char *name2;
    int fd,fd1,debug = 0;
    int flag1,flag2 = 0;
    char buff[1] = "\n";
    int i;
    for(i = 0; i < argc; i++){
        if(!strncmp(argv[i], "-D",2))
            debug = 1;
        if(!strncmp(argv[i],"-o",2)){
            outFile = system_call(OPEN,argv[i]+2,O_CREATE | O_WRONLY, DEFAULT);
            name1 = argv[i]+2;
            flag1 = 1;
        }
        if(!strncmp(argv[i], "-i",2)){
            inFile = system_call(OPEN,argv[i]+2,O_RDONLY, DEFAULT);
            name2 = argv[i]+2;
            flag2 = 1;
        }       
    }
    if(debug && flag1){
        system_call(WRITE,STDERR,name1,strlen(name1));
        system_call(WRITE,STDERR,buff,1);
        system_call(WRITE,STDERR," Call id: ",11);
        system_call(WRITE,STDERR,itoa(OPEN),1);
        system_call(WRITE,STDERR,", Return value: ",16);
        system_call(WRITE,STDERR,itoa(outFile),1);
        system_call(WRITE,STDERR,buff,1);
    }
    if(debug && flag2){
        system_call(WRITE,STDERR,name2,strlen(name2));
        system_call(WRITE,STDERR,buff,1);
        system_call(WRITE,STDERR," Call id: ",11);
        system_call(WRITE,STDERR,itoa(OPEN),1);
        system_call(WRITE,STDERR,", Return value: ",16);
        system_call(WRITE,STDERR,itoa(inFile),1);
        system_call(WRITE,STDERR,buff,1);
    }
    if(!flag1 && debug)
        system_call(WRITE,STDERR,"stdout  ",8);
    if(!flag2 && debug)
        system_call(WRITE,STDERR,"stdin\n",6);

    if(outFile < 0 || inFile < 0)
        system_call(EXIT, COD, 0, 0);
    char c[1];
    while((fd = system_call(READ,inFile, c, 1)) > 0){
        if(c[0] >= 'A' && c[0] <= 'Z')
            c[0] = c[0] + ('a'-'A');
        fd1 = system_call(WRITE,outFile,c,1);
        if(debug && c[0] != '\n'){
            system_call(WRITE,STDERR," Call id: ",11);
            system_call(WRITE,STDERR,itoa(READ),1);
            system_call(WRITE,STDERR,", Return value: ",16);
            system_call(WRITE,STDERR,itoa(fd),1);
            system_call(WRITE,STDERR,buff,1);
            system_call(WRITE,STDERR,"  Call id: ",11);
            system_call(WRITE,STDERR,itoa(WRITE),1);
            system_call(WRITE,STDERR,", Return value: ",16);
            system_call(WRITE,STDERR,itoa(fd1),1);
            system_call(WRITE,STDERR,buff,1);
        }
        if(fd <0 || fd1 <0)
            system_call(EXIT,COD,0,0);
    }
    return 0;
}