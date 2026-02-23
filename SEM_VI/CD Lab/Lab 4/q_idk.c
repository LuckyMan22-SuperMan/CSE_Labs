#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 100

/* ---------- token ---------- */
typedef struct
{
    char lexeme[100];
    char type[50];
    int row, col;
} token;

/* ---------- keyword list ---------- */
char keywords[32][10] =
{
    "auto","break","case","char","const","continue","default","do",
    "double","else","enum","extern","float","for","goto","if",
    "int","long","register","return","short","signed","sizeof","static",
    "struct","switch","typedef","union","unsigned","void","volatile","while"
};

int row = 1, col = 0;

/* ---------- keyword check ---------- */
int isKeyword(char *word)
{
    for(int i = 0; i < 32; i++)
    {
        if(strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/*Cheching datatype for symbol table*/
int isDataType(char *s)
{
    char *types[] = {
        "int","float","double",
        "char","void","long","short"
    };

    for(int i=0;i<7;i++)
        if(!strcmp(s, types[i]))
            return 1;

    return 0;
}


/* ---------- skip helpers ---------- */
void skipPreprocessor(FILE *fp)
{
    char c;
    while((c = fgetc(fp)) != '\n' && c != EOF);
    
    row++; 
    col = 0;
}

void skipSingleComment(FILE *fp)
{
    char c;
    while((c = fgetc(fp)) != '\n' && c != EOF);
    
    row++; 
    col = 0;
}

void skipMultiComment(FILE *fp)
{
    char c, prev = 0;
    while((c = fgetc(fp)) != EOF)
    {
        if(c == '\n')
        {
            row++;
            col = 0;
        }
        if(prev == '*' && c == '/')
            break;
        prev = c;
    }
}

void skipString(FILE *fp)
{
    char c;
    while((c = fgetc(fp)) != EOF)
    {
        if(c == '\\')
            fgetc(fp);
        else if(c == '"')               //string ending condition
            break;
        else if(c == '\n')
        {
            row++;
            col = 0;
        }
    }
}

/* ---------- lexer ---------- */
token getNextToken(FILE *fp)
{
    token t;
    char c, buf[100];
    int i = 0;

    while((c = fgetc(fp)) != EOF)
    {
        col++;

        if(isspace(c))
        {
            if(c == '\n')
            {
                row++;
                col = 0;
            }
            continue;
        }

        if(c == '#')
        {
            skipPreprocessor(fp);
            continue;
        }

        if(c == '/')
        {
            char next = fgetc(fp);

            if(next == '/')
            {
                skipSingleComment(fp);
                continue;
            }
            else if(next == '*')
            {
                skipMultiComment(fp);
                continue;
            }
            else
            {
                ungetc(next, fp);
            }
        }

        if(c == '"')
        {
            skipString(fp);
            continue;
        }

        t.row = row;
        t.col = col;

        if(isalpha(c) || c == '_')              //identifier or keyword
        {
            buf[i++] = c;

            while(isalnum(c = fgetc(fp)) || c == '_')
            {
                buf[i++] = c;
                col++;
            }

            buf[i] = '\0';
            ungetc(c, fp);

            strcpy(t.lexeme, buf);

            if(isKeyword(buf))
                strcpy(t.type, "KEYWORD");
            else
                strcpy(t.type, "IDENTIFIER");

            return t;
        }

        if(isdigit(c))
        {
            buf[i++] = c;

            while(isdigit(c = fgetc(fp)))
            {
                buf[i++] = c;
                col++;
            }

            buf[i] = '\0';
            ungetc(c, fp);

            strcpy(t.lexeme, buf);
            strcpy(t.type, "NUMBER");
            return t;
        }

        t.lexeme[0] = c;
        t.lexeme[1] = '\0';
        strcpy(t.type, "SYMBOL");
        return t;
    }

    strcpy(t.type, "EOF");
    return t;
}

/* ---------- symbol tables ---------- */

typedef struct
{
    char funcName[50];
    char returnType[20];
    char args[10][50];
    char argTypes[10][20];
    int argCount;
} GlobalSymbol;

typedef struct
{
    char funcName[50];
    char varName[50];
    char varType[20];
} LocalSymbol;

GlobalSymbol gTable[50];
LocalSymbol lTable[200];

int gCount = 0;
int lCount = 0;

/* ---------- build tables ---------- */
void buildSymbolTables(FILE *fp)
{
    token t, next;
    char currentType[20] = "";
    char currentFunction[50] = "";
    int insideFunction = 0;

    while(1)
    {
        t = getNextToken(fp);
        if(strcmp(t.type, "EOF") == 0)
            break;

        if(strcmp(t.type,"KEYWORD")==0 && isDataType(t.lexeme))
        {
            strcpy(currentType, t.lexeme);              //return type

            next = getNextToken(fp);

            if(strcmp(next.type,"IDENTIFIER")==0)
            {
                token third = getNextToken(fp);

                /* function */
                if(strcmp(third.lexeme,"(")==0)
                {
                    GlobalSymbol *g = &gTable[gCount];

                    strcpy(g->funcName, next.lexeme);
                    strcpy(g->returnType, currentType);

                    g->argCount = 0;

                    strcpy(currentFunction, next.lexeme);
                    insideFunction = 1;

                    token argType, argName;

                    while(1)
                    {
                        argType = getNextToken(fp);
                        if(strcmp(argType.lexeme,")")==0)
                            break;

                        if(strcmp(argType.type,"KEYWORD")==0)
                        {
                            argName = getNextToken(fp);

                            if(strcmp(argName.type,"IDENTIFIER")==0)
                            {
                                strcpy(g->args[g->argCount], argName.lexeme);
                                strcpy(g->argTypes[g->argCount], argType.lexeme);
                                g->argCount++;
                            }
                        }
                    }

                    gCount++;
                }
                else if(insideFunction)
                {
                    strcpy(lTable[lCount].funcName,currentFunction);
                    strcpy(lTable[lCount].varName,next.lexeme);
                    strcpy(lTable[lCount].varType,currentType);

                    lCount++;
                }
            }
        }

        if(strcmp(t.lexeme,"}")==0)
            insideFunction = 0;
    }
}

/* ---------- print tables ---------- */
void printTables()
{
    printf("\nGLOBAL SYMBOL TABLE\n");
    for(int i = 0; i < gCount; i++)
    {
        printf("\nFunction: %s", gTable[i].funcName);
        printf("\nReturn Type: %s", gTable[i].returnType);

        printf("\nArguments: ");
        for(int j = 0; j < gTable[i].argCount; j++)
        {
            printf("%s(%s) ",
                   gTable[i].args[j],
                   gTable[i].argTypes[j]);
        }
        printf("\n");
    }

    printf("\nLOCAL SYMBOL TABLE\n");
    for(int i = 0; i < lCount; i++)
    {
        printf("%s : %s (%s)\n", lTable[i].funcName, lTable[i].varName, lTable[i].varType);
    }
}

/* ---------- main ---------- */
int main()
{
    FILE *fp;
    char filename[100];

    printf("Enter filename: ");
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if(!fp)
    {
        printf("File not found");
        return 0;
    }

    buildSymbolTables(fp);
    printTables();

    fclose(fp);
    return 0;
}
