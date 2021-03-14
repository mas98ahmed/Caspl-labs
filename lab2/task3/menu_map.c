#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* map(char *array, int array_length, char (*f) (char));

char censor(char c);

char encrypt(char c);/* Gets a char c and returns its encrypted form by adding 3 to its value. 
          If c is not between 0x20 and 0x7E it is returned unchanged */

char decrypt(char c); /* Gets a char c and returns its decrypted form by reducing 3 to its value. 
            If c is not between 0x20 and 0x7E it is returned unchanged */

char cprt(char c); /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */

char my_get(char c); /* Ignores c, reads and returns a character from stdin using fgetc. */


struct fun_desc{
    char *name;
    char (*fun)(char);
};

void print_menu(struct fun_desc men[]);

int main(int argc, char **argv){
    struct fun_desc menu[] = {{"Censor", censor}, {"Encrypt", encrypt}, {"Decrypt", decrypt},
                             {"Print string", cprt}, {"Get string", my_get}};
    int length = 5;
    int MaxLength = 1024;
    char carray[length];
    char op[MaxLength];
    while (1){
        print_menu(menu);
        printf("Option: ");
        scanf("%s",op);
        fgetc(stdin);
        if(strcmp(op,"0") >= 0 && strcmp("4",op) >= 0){
            printf("Within bounds\n");
            int func_num = atoi(op);
            strcpy(carray,map(carray,length,menu[func_num].fun));
            printf("DONE\n\n");
        }else{
            printf("Not within bounds\n");
            exit(0);
        }
    }
}

void print_menu(struct fun_desc men[]){
    int i = 0;
    printf("please choose a function: \n");
    while(i < 5){
        printf("%d) %s\n",i,men[i].name);
        i++;
    }
}

char* map(char *array, int array_length, char (*f) (char)){
    char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
    for (int i = 0; i < array_length; i++){
        mapped_array[i] = f(array[i]);
    }
    return mapped_array;
}

char censor(char c) {
    if(c == '!')
        return '.';
    else
        return c;
}

char encrypt(char c){
    if (0x20 <= c && c <= 0x7E){
        return c+3;
    }
    return c;
}

char decrypt(char c){
    if (0x20 <= c && c <= 0x7E){
        return c-3;
    }
    return c;
}

char cprt(char c){
    if( c >= 0x20 && c <= 0x7E)
        printf("%c\n",c);
    else
        printf(".\n");
    return c ;
}

char my_get(char c){
    int ch = fgetc(stdin);
    return ch;
}