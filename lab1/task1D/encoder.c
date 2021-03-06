#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    int ch;
    int debug = 0;
    int letter_count = 0;
    int encry = 0;
    int pos = 0;
    int neg = 0;
    char key = ' ';
    FILE *input = stdin;
    FILE *output = stdout;

    /* checking the arguments */
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"-D") == 0){
            debug = 1;
            fprintf(stderr,"-D\n");
        }
        if(argv[i][1] == 'e'){
            encry = 1;
            pos = argv[i][0] == '+' ? 1 : 0;
            neg = argv[i][0] == '-' ? 1 : 0;
            if(strlen(argv[i]) == 3){
                key = argv[i][2];
            }
        }
        if(argv[i][1] == 'i'){
            char *infile = argv[i];
            infile += 2;
            input = fopen(infile,"r");
        }
    }

    /* starting the action of the program */
    while((ch = fgetc(input)) != EOF){
        char b = ch;
        if('A' <= ch && ch<='Z'){
            b = (ch - 'A') + 'a';
            letter_count++;
        }

        /* this works if we entered "encoder -D" */
        if(debug == 1){ 
            if (ch == 10){
                fprintf(stderr,"\nthe number of letters: %d\n\n",letter_count);
            }else{
                fprintf(stderr, ((int)ch) < 100 ? "%d    %d\n" : "%d   %d\n",(int)ch,(int)b);
            }
        }
        /**/
        if(encry == 1){
            /* finding the hexadecimal value */
            int value = 0;
            if('0' <= key && key <= '9'){
                value = key - '0';
            }else{
                value = key - 'A' + 10;
            }
            /* start the encrypting */
            if(pos == 1){
                if(key != ' ' && ('A' <= ch && ch<='Z')){
                    b = (ch + value) % 127;
                }else{
                    if('A' <= ch && ch<='Z'){
                        b = (ch - 'A') + 'a';
                    }
                }
            }
            if (neg == 1){
                if(key != ' '){
                    b = (ch - value) % 127;
                }else{
                    if('A' <= ch && ch<='Z'){
                        b = (ch - 'A') + 'a';
                    }
                }
            }     
        }
        fputc(b,output);
    }
    fputc('\n',output);
    fclose(output);
}