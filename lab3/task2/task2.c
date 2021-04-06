#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};

struct fun_desc {
  char *name;
  void (*fun)(int);
};

void Load_signatures(int opcode);
void print_signatures(int opcode);
void virusDetection(int opcode);
void detect_virus(char *buffer, unsigned int size, link *virus_list, FILE* output, int opcode);
void kill_virus(char *fileName, int signnitureOffset, int signitureSize);
void print_menu(struct fun_desc menu[]);
void list_print(link *virus_list, FILE* output);
link* list_append(link* virus_list, virus* data);
void list_free(link *virus_list);
virus* readVirus(FILE* input);
void printVirus(virus* virus, FILE* output);
void PrintHex(FILE* output,unsigned char* buffer,int length);
int input_entered = 0;
int first_time = 1;
link* Head = NULL;
char* infectedFile;
int _argc = 0;

int main(int argc, char* argv[]){
    if(argc>1){
        _argc=1;
        infectedFile=argv[1];
    }
    struct fun_desc menu[] = {{"Load signatures",Load_signatures},{"print signatures",print_signatures},
                                {"Detect viruses",virusDetection},{"kill_virus",virusDetection},{NULL,NULL}};
    while(1){
        print_menu(menu);
        char op[1];
        scanf("%s",op);
        if(strcmp(op,"q") == 0){
            break;
        }else{
            int choose = atoi(op);
            menu[choose-1].fun(choose);
        }
    }
    if(Head != NULL)
        list_free(Head);
    return 0;
}


void Load_signatures(int opcode){
    char fileName[50];
    int count = 0;
    printf("Enter signature file name: ");
    fgetc(stdin);
    fgets(fileName,50,stdin);
    for(int i = 0; i < 50; i++){
        if(fileName[i]=='\0')
            break;
        count++;
    }
    for(int i = count; i < 50; i++){
        fileName[i] = 0;
    }
    char* name = (char*)malloc( count*sizeof(char));
    strncpy(name,fileName,count-1);
    link* virus_list = NULL;
    FILE *input = fopen(name,"r");
    if (input != 0){
        virus* v;
        while((v = readVirus(input)) != NULL){
            virus_list = list_append(virus_list,v);  
        }
        Head = virus_list;
        fclose(input);
        input_entered = 1;
    }
    free(name);
}

void print_signatures(int opcode){
    if(input_entered){
        FILE *output = fopen("lab3_out","w+");
        list_print(Head, output);
        fclose(output);
    }
}

void virusDetection(int opcode){
    if(_argc){
        int fileSize=0;
        FILE* fp = fopen(infectedFile,"rb");
        FILE* output = fopen("lab3_detection_out","w+");
        if(fp){

            char* buffer = calloc(100000 , sizeof(char));
            fseek(fp, 0, SEEK_END); 
            fileSize = ftell(fp);
            rewind(fp);
            fread(buffer, 1,fileSize, fp);
            detect_virus(buffer,fileSize,Head,output,opcode);
            free(buffer);
        }
        fclose(fp);
        fclose(output);
    }
}

void detect_virus(char *buffer, unsigned int size, link *virus_list, FILE* output,int opcode){
    int i=0;
    link* virusPtr=virus_list;
    while(i<size){
        while(virusPtr!=NULL){
            if(memcmp(buffer+i,virusPtr->vir->sig,virusPtr->vir->SigSize)==0){
                if(opcode == 3){
                    fprintf(output,"The starting byte location: %d\n",i);
                    fprintf(output,"The virus name: %s\n",virusPtr->vir->virusName);
                    fprintf(output,"The size of the virus signature: %d\n",virusPtr->vir->SigSize);
                }else{
                    kill_virus(infectedFile,i,virusPtr->vir->SigSize);
                }
            }
            virusPtr=virusPtr->nextVirus;
        }
        i++;
        virusPtr=virus_list;
    }   
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize){
        FILE* fp;
        char _replace[signitureSize];
        fp = fopen(fileName,"rb+");
        fseek(fp, signitureOffset,SEEK_CUR);
        for(int i=0;i<signitureSize;i++)
            _replace[i]=(char)0x90;
        fwrite(_replace , 1 , sizeof(_replace)/sizeof(char), fp);       
        fclose(fp);
}

void print_menu(struct fun_desc menu[]){
    int i = 1;
    while (menu[i-1].name != NULL){
        printf("%d) %s\n",i,menu[i-1].name);
        i++;
    }
}

void list_print(link *virus_list, FILE* output){
    link* l = virus_list;
    while(l != NULL){
        printVirus(l->vir,output);
        l = l->nextVirus;
    }
}

link* list_append(link* virus_list, virus* data){
    if(virus_list == NULL){
        virus_list = (link*)malloc(sizeof(link*));
        virus_list->vir = data;
    }else{
        link* l = virus_list;
        while(l->nextVirus != NULL){
            l = l->nextVirus;
        }
        l->nextVirus = (link*)malloc(sizeof(link*));
        l->nextVirus->vir = data;
    }
    return virus_list;
}

void list_free(link *virus_list){
    link* ptr=virus_list;
    link* temp=ptr;
    while(ptr){
        ptr=ptr->nextVirus;
        free(temp->vir->sig);
        free(temp->vir);
        free(temp);
        temp=ptr;   
    }
}

virus* readVirus(FILE* input){
    virus* v = (virus*)malloc(sizeof(virus));
    if(first_time){
        char n[4];
        fread(n,1,4,input);
        first_time = 0;
    }
    if(fread(v,1,18,input)){
        v->sig = (unsigned char*)malloc(sizeof(char)*(v->SigSize+1));
        unsigned char name[v->SigSize];
        fread(name,1,v->SigSize,input);
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