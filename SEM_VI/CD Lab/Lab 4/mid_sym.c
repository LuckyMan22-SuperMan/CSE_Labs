#include<stdio.h>
#include<string.h>
#include "supp.h"
#include<stdlib.h>

typedef struct
{
    char name[1024];
    char type[1024];
    char arg[1024];
}table;

table tb[100];
int ind=0;

void insert(token tk, char *arg)
{
    for(int i=0;i<ind;i++)
    {
        if(strcmp(tk.name, tb[i].name)==0) 
            return;
    }
    strcpy(tb[ind].name, tk.name);

    if(checkFunc(tk.name)) 
        strcpy(tb[ind].type, "FUNC");
    else 
        strcpy(tb[ind].type, tk.type);

    strcpy(tb[ind].arg, arg);
    ind++;
}

void display(){
    printf("Token Name\tToken Type\tArguments\n");
    for(int i=0;i<ind;i++)
    {
        printf("%s\t%s\t%s\n", tb[i].name, tb[i].type, tb[i].arg);
    }
}

int main(){
    char name[1024];
    printf("enter file name\n");
    gets(name);
    FILE *fp = fopen(name, "r");

    if(!fp)
    {
        printf("error openeing file");
        exit(1);
    }
    while(1)
    {
        token tk= getNextToken(fp);
        if(strcmp(tk.type, "EOF")==0) 
            break;
        if(strcmp(tk.type, "IDENTIFIER")==0)
        {
            if(checkFunc(tk.name))
            {
                token next = getNextToken(fp);

                if(strcmp(next.name,"(")==0)
                {
                    char buf[1024]="";
                    token temp;

                    while(1)
                    {
                        temp=getNextToken(fp);
                        if(strcmp(temp.name,")")==0) break;

                        strcat(buf,temp.name);
                        strcat(buf," ");
                    }
                    insert(tk, buf);
                }
            }
            else 
                insert(tk, "");
        }
    }
    rewind(fp);

    while(1)
    {
        token tk= getNextToken(fp);
        if(strcmp(tk.type, "EOF")==0) 
            break;
        printf("<%s, %s, %d, %d>\n", tk.name, tk.type, tk.row, tk.col);
    }
    display();
    fclose(fp);
    return 0;

}