#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "la.h"

#define MAX_SYMBOLS 100
#define MAX_FUNCS 20


struct entry
{
    char name[30];
    char dtype[20];
};

struct functionTable
{
    char fname[30];
    struct entry locals[MAX_SYMBOLS];
    int localCount;
};

char globalFuncs[MAX_FUNCS][30];
int globalCount = 0;

struct functionTable funcTables[MAX_FUNCS];
int funcCount = 0;

char currentDtype[20] = "NULL";

int scopeLevel = 0;
int inDeclaration = 0;

/* datatype list */
const char *Datatypes[] =
{"int","float","char","double","void","bool"};

int isDtype(char *str)
{
    for(int i=0;i<6;i++)
        if(strcmp(str,Datatypes[i])==0)
            return 1;
    return 0;
}

/* add function */
void insertFunction(char *name)
{
    strcpy(globalFuncs[globalCount], name);

    strcpy(funcTables[funcCount].fname, name);
    funcTables[funcCount].localCount = 0;

    globalCount++;
    funcCount++;
}

/* add local variable */
void insertLocal(char *name)
{
    struct functionTable *ft = &funcTables[funcCount-1];

    for(int i=0;i<ft->localCount;i++)
        if(strcmp(ft->locals[i].name,name)==0)
            return;

    strcpy(ft->locals[ft->localCount].name,name);
    strcpy(ft->locals[ft->localCount].dtype,currentDtype);
    ft->localCount++;
}

/* display tables */
void displayTables()
{
    printf("\nGLOBAL SYMBOL TABLE\n");
    printf("SlNo\tFunction\n");

    for(int i=0;i<globalCount;i++)
        printf("%d\t%s\n",i+1,globalFuncs[i]);

    printf("\nLOCAL SYMBOL TABLES\n");

    for(int i=0;i<funcCount;i++)
    {
        printf("\nFunction: %s\n",funcTables[i].fname);
        printf("No\tName\tType\n");

        for(int j=0;j<funcTables[i].localCount;j++)
            printf("%d\t%s\t%s\n",
                   j+1,
                   funcTables[i].locals[j].name,
                   funcTables[i].locals[j].dtype);
    }
}

int main()
{
    FILE *fp = fopen("demo.c","r");
    if(!fp)
    {
        printf("File open error\n");
        return 1;
    }

    struct token tkn, prev;
    prev.lexeme[0]='\0';

    while(1)
    {
        tkn = getNextToken(fp);
        if(strcmp(tkn.type,"EOF")==0)
            break;

        /* datatype detected */
        if(strcmp(tkn.type,"Keyword")==0 && isDtype(tkn.lexeme))
        {
            strcpy(currentDtype,tkn.lexeme);
            inDeclaration = 1;
        }

        /* detect function name */
        if(strcmp(prev.type,"Keyword")==0 &&
           strcmp(tkn.type,"Identifier")==0)
        {
            struct token next = getNextToken(fp);

            if(strcmp(next.lexeme,"(")==0)
            {
                insertFunction(tkn.lexeme);
                scopeLevel = 0;
                inDeclaration = 0;
            }

            fseek(fp,-1,SEEK_CUR);
        }

        /* scope tracking */
        if(strcmp(tkn.lexeme,"{")==0)
            scopeLevel++;

        if(strcmp(tkn.lexeme,"}")==0)
            scopeLevel--;

        /* end declaration */
        if(strcmp(tkn.lexeme,";")==0)
            inDeclaration = 0;

        /* insert variable only in declaration */
        if(scopeLevel>0 &&
           inDeclaration &&
           strcmp(tkn.type,"Identifier")==0)
        {
            insertLocal(tkn.lexeme);
        }

        prev = tkn;
    }

    fclose(fp);
    displayTables();
    return 0;
}
