#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

#define size 1024

char keywords[32][10] = {
    "auto", "break", "case", "char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register", "return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

int checkKW(char word[]){
    for(int i=0;i<32;i++){
        if(strcmp(keywords[i], word)==0) 
            return 1;
    }
    return 0;
}
int main(){
    FILE *inp;
    char inpf[size];
    printf("enter the input file\n");
    scanf("%s", inpf);
    inp = fopen(inpf, "r");

    char c = fgetc(inp);
    char word[size];
    int i=0;
    while(c!=EOF)
    {
        if(!isalnum(c) && c != '_')
        {
            word[i]='\0';
            if(checkKW(word)) 
                printf(" found a keyword %s\n", strupr(word));
            
            c=fgetc(inp);
            i=0;
        }
        else
        {
            word[i++]=c;
            c=fgetc(inp);
        }

    }
    fclose(inp);
    return 0;



}