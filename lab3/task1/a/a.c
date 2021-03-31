#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

virus* readVirus(FILE* f);
void printVirus(virus* virus, FILE* output);
void PrintHex(FILE* output,unsigned char* buffer,int length);
int first_time = 1;

int main(int argc, char* argv[]){
    FILE *input = fopen("signatures-L","r+");        
    FILE *output = fopen("signatures","w+");
    virus* v;
    while((v =readVirus(input)) != NULL){
        printVirus(v,output);
    }    
    fclose(input);
    fclose(output);
    return 0;
}

virus* readVirus(FILE* f){
    virus* v = (virus*)malloc(sizeof(virus));
    if(first_time){
        char n[4];
        fread(n,1,4,f);
        first_time = 0;
    }
    if(fread(v,1,18,f)){
        v->sig = (unsigned char*)malloc(sizeof(char)*(v->SigSize+1));
        unsigned char name[v->SigSize];
        fread(name,1,v->SigSize,f);
        memcpy(v->sig,name,sizeof(name));
        return v;
    }else{
        free(v);
        return NULL;
    }
}

void printVirus(virus* virus, FILE* output){
    fprintf(output,"Virus name: %s\n",virus->virusName);
    fprintf(output,"Virus size: %d\n",virus->SigSize);
    fprintf(output,"signature:\n");
    PrintHex(output,virus->sig,virus->SigSize);
    fprintf(output,"\n\n");
}


void PrintHex(FILE* output, unsigned char* buffer,int length){
    for( int i =0; i < length ;i++){
        fprintf(output,"%02X ",buffer[i] & 0xff);
    }
}