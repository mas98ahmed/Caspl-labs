#include "util.h"
#define WRITE 4
#define READ 3

#define STDERR 2
#define STDOUT 1
#define STDIN 0

extern int system_call();

int main(int argc, char** argv){
    int outFile = STDOUT;
    int inFile = STDIN;
    int fd,fd1,debug = 0;
    char buff[1] = "\n";
    
    int i;
    for(i = 0; i < argc; i++){
        if(!strncmp(argv[i], "-D",2))
            debug = 1;       
    }

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
    }
    return 0;
}