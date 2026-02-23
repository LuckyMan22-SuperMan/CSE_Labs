#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 100

typedef struct
{
    char lexeme[100];
    char type[50];
    int row, col;
} token;


/* =====================================================
   KEYWORD LIST
   MODIFY THIS if exam uses different keywords
   (JS keywords, custom keywords, etc.)
   ===================================================== */
char keywords[32][10] =
{
    "auto","break","case","char","const","continue","default","do",
    "double","else","enum","extern","float","for","goto","if",
    "int","long","register","return","short","signed","sizeof","static",
    "struct","switch","typedef","union","unsigned","void","volatile","while"
};


/* =====================================================
   GLOBAL POSITION TRACKING
   Used for row/column printing.
   ===================================================== */
int row = 1, col = 0;



int isKeyword(char word[])
{
    for(int i = 0; i < 32; i++)
    {
        if(strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}


void skipPreprocessor(FILE *fp)
{
    char c;

    while((c = fgetc(fp)) != '\n' && c != EOF)
    {
    }

    row++;
    col = 0;
}



void skipSingleComment(FILE *fp)                    //skip single line comments
{
    char c;

    while((c = fgetc(fp)) != '\n' && c != EOF)
    {}

    row++;
    col = 0;
}


void skipMultiComment(FILE *fp)                     //skip mulyi line comments
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
        {
            break;
        }

        prev = c;
    }
}



void skipString(FILE *fp)
{
    char c;

    while((c = fgetc(fp)) != EOF)
    {
        if(c == '\\')       // skip escaped characters
        {
            fgetc(fp);
        }
        else if(c == '"')   // end of string
        {
            break;
        }
        else if(c == '\n')
        {
            row++;
            col = 0;
        }
    }
}


token getNextToken(FILE *fp)
{
    token t;
    char c, buf[100];
    int i = 0;

    while((c = fgetc(fp)) != EOF)
    {
        col++;

        /* ---------- skip whitespace ---------- */
        if(isspace(c))
        {
            if(c == '\n')
            {
                row++;
                col = 0;
            }
            continue;
        }

        /* ---------- skip preprocessor ---------- */
        if(c == '#')
        {
            skipPreprocessor(fp);
            continue;
        }

        /* ---------- skip comments ---------- */
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

        /* ---------- skip strings ---------- */
        if(c == '"')
        {
            skipString(fp);
            continue;
        }

        /* ---------- store token position ---------- */
        t.row = row;
        t.col = col;


        /* ---------- IDENTIFIER / KEYWORD ---------- */
        /* MODIFY HERE if identifiers rules change */
        if(isalpha(c) || c == '_')
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


        /* ---------- NUMBER ---------- */
        /* MODIFY HERE if floats allowed */
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


        /* ---------- SYMBOL / OPERATOR ---------- */
        /* MODIFY HERE to detect ==, <=, && etc. */
        /* ---------- OPERATORS & SYMBOLS ---------- */
        char next;

        /* arithmetic, logical, relational operators */
        if(c=='+' || c=='-' || c=='=' || c=='!' ||
        c=='<' || c=='>' || c=='&' || c=='|')
        {
            next = fgetc(fp);

            /* ++ or -- */
            if((c=='+' && next=='+') ||
            (c=='-' && next=='-'))
            {
                t.lexeme[0] = c;
                t.lexeme[1] = next;
                t.lexeme[2] = '\0';
                strcpy(t.type, "OPERATOR");
                col++;
                return t;
            }

            /* ==, !=, <=, >= */
            if(next == '=')
            {
                t.lexeme[0] = c;
                t.lexeme[1] = next;
                t.lexeme[2] = '\0';
                strcpy(t.type, "OPERATOR");
                col++;
                return t;
            }

            /* && or || */
            if((c=='&' && next=='&') ||
            (c=='|' && next=='|'))
            {
                t.lexeme[0] = c;
                t.lexeme[1] = next;
                t.lexeme[2] = '\0';
                strcpy(t.type, "OPERATOR");
                col++;
                return t;
            }

            /* single operator */
            ungetc(next, fp);

            t.lexeme[0] = c;
            t.lexeme[1] = '\0';
            strcpy(t.type, "OPERATOR");
            return t;
        }

        /* remaining symbols */
        t.lexeme[0] = c;
        t.lexeme[1] = '\0';
        strcpy(t.type, "SYMBOL");
        return t;

    }

    strcpy(t.type, "EOF");
    return t;
}


/* =====================================================
   MAIN DRIVER
   Usually unchanged in exam.
   ===================================================== */
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

    token t;

    while(1)
    {
        t = getNextToken(fp);

        if(strcmp(t.type, "EOF") == 0)
        {
            break;
        }

        /* MODIFY PRINT FORMAT IF REQUIRED */
        printf("< %s , %s , %d , %d >\n",
               t.lexeme, t.type, t.row, t.col);
    }

    fclose(fp);
    return 0;
}
