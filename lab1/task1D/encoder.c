#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    int ch;
    int debug = 0;
    int letters = 0;
    int encry = 0;
    int pos = 0;
    int neg = 0;
    char key = ' ';
    FILE *input = stdin;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"-D") == 0){
            debug = 1;
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
            infile+=2;
            input = fopen(infile,"r");
        }
        fprintf(stderr,"%s\n",argv[i]);
    }

    if(input != NULL){
        int ch_new;
        int old_letters = letters;
        while((ch = fgetc(input)) != EOF){
            ch_new = ch;
            old_letters = letters;
            if('A' <= ch && ch<='Z'){
                    ch_new = (ch - 'A') + 'a';
                    letters++;
            }
            if(debug == 1){ 
                if (ch == 10){
                    fprintf(stderr,"\nthe number of letters: %d\n\n",letters);
                }else{
                    fprintf(stderr, ch < 100 ? "%d    %d\n" : "%d   %d\n",ch,ch_new);
                }
            }
            //********
            if(encry == 1){
                int value = 0;
                if('0' <= key && key <= '9'){
                    value = key - '0';
                }else{
                    value = key - 'A' + 10;
                }
                if(pos == 1){
                    if(key != ' '){
                        ch_new = (ch + value > 127 ? ch - value : ch + value) % 127;
                    }else{
                        if('A' <= ch && ch<='Z'){
                            ch_new = (ch - 'A') + 'a';
                        }
                    }
                }
                if (neg == 1){
                    if(key != ' '){
                        ch_new = (ch + value < 0 ? 127 - (value - ch) : ch - value) % 127;
                    }else{
                        if('A' <= ch && ch<='Z'){
                            ch_new = (ch - 'A') + 'a';
                        }
                    }
                }
                if(old_letters == letters){
                    letters++;     
                }
            }
            if(ch != 10){
                fprintf(stdout,"%c",ch_new);
            }else{
                fprintf(stdout,"\n");
            }
        }
        fclose(input);
    }else{
        fprintf(stderr,"***there is a problem***\n");
    }
}