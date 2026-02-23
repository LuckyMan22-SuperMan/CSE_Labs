#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define bufsiz 2024

typedef struct{
    char name[100];
    unsigned int row, col;
    char type[30];
} token;

char keywords[32][10] = {
    "auto","break","case","char","const","continue",
    "default","do","double","else","enum","extern",
    "float","for","goto","if","int","long",
    "register","return","short","signed","sizeof",
    "static","struct","switch","typedef","union",
    "unsigned","void","volatile","while"
};

int checkKW(char word[])
{
    for(int i=0;i<32;i++)
        if(strcmp(keywords[i], word)==0)
            return 1;
    return 0;
}

/* ---------- lexer ---------- */
token getNextToken(FILE *input)
{
    static int row=1, col=1;
    static int lastPrintedRow = 0;

    char c;
    token t;
    t.type[0] = '\0';

    while((c=fgetc(input))!=EOF)
    {
        char buf[100];
        int i=0;

        /* ---------- preprocessor ---------- */
        if(c=='#')
        {
            while((c=fgetc(input))!='\n' && c!=EOF);
            row++; col=1;
            continue;
        }

        /* ---------- newline ---------- */
        if(c=='\n')
        {
            row++;
            col=1;
            continue;
        }

        /* ---------- skip string ---------- */
        if(c=='"')
        {
            col++;
            c=fgetc(input);
            while(c!='"' && c!=EOF)
            {
                if(c=='\n'){
                    row++;
                    col=1;
                }
                else col++;

                c=fgetc(input);
            }
            col++;
            continue;
        }

        /* ---------- comments ---------- */
        if(c=='/')
        {
            char next=fgetc(input);

            if(next=='/')
            {
                while((c=fgetc(input))!='\n' && c!=EOF);
                row++; col=1;
                continue;
            }
            else if(next=='*')
            {
                char prev=0;
                col+=2;
                while((c=fgetc(input))!=EOF)
                {
                    if(c=='\n'){
                        row++;
                        col=1;
                    }
                    else col++;

                    if(prev=='*' && c=='/')
                        break;

                    prev=c;
                }
                continue;
            }
            else
            {
                t.row=row;
                t.col=col;
                strcpy(t.name,"/");
                strcpy(t.type,"arith");
                col++;
                ungetc(next,input);
                return t;
            }
        }

        /* ---------- numbers ---------- */
        if(isdigit(c))
        {
            int startCol=col;
            buf[i++]=c;
            col++;

            c=fgetc(input);
            while(isdigit(c))
            {
                buf[i++]=c;
                col++;
                c=fgetc(input);
            }

            buf[i]='\0';
            ungetc(c,input);

            t.row=row;
            t.col=startCol;
            strcpy(t.name,buf);
            strcpy(t.type,"num");
            return t;
        }

        /* ---------- identifiers & keywords ---------- */
        if(isalpha(c) || c=='_')
        {
            int startCol=col;
            buf[i++]=c;
            col++;

            c=fgetc(input);
            while(isalnum(c) || c=='_')
            {
                buf[i++]=c;
                col++;
                c=fgetc(input);
            }

            buf[i]='\0';
            ungetc(c,input);

            t.row=row;
            t.col=startCol;
            strcpy(t.name,buf);

            if(checkKW(buf))
                strcpy(t.type,"keyword");
            else
                strcpy(t.type,"id");

            return t;
        }

        /* ---------- arithmetic ---------- */
        if(c=='+' || c=='-' || c=='*')
        {
            char next=fgetc(input);

            t.row=row;
            t.col=col;

            if((c=='+' && next=='+') ||
               (c=='-' && next=='-'))
            {
                t.name[0]=c;
                t.name[1]=next;
                t.name[2]='\0';
                col+=2;
            }
            else
            {
                t.name[0]=c;
                t.name[1]='\0';
                col++;
                ungetc(next,input);
            }

            strcpy(t.type,"arith");
            return t;
        }

        /* ---------- relational ---------- */
        if(c=='<' || c=='>' || c=='=' || c=='!')
        {
            char next=fgetc(input);

            t.row=row;
            t.col=col;

            if(next=='=')
            {
                t.name[0]=c;
                t.name[1]=next;
                t.name[2]='\0';
                col+=2;
            }
            else
            {
                t.name[0]=c;
                t.name[1]='\0';
                col++;
                ungetc(next,input);
            }

            strcpy(t.type,"rel");
            return t;
        }

        /* ---------- logical ---------- */
        if(c=='&' || c=='|')
        {
            char next=fgetc(input);

            t.row=row;
            t.col=col;

            if(next==c)
            {
                t.name[0]=c;
                t.name[1]=next;
                t.name[2]='\0';
                col+=2;
            }
            else
            {
                t.name[0]=c;
                t.name[1]='\0';
                col++;
                ungetc(next,input);
            }

            strcpy(t.type,"log");
            return t;
        }

        /* ---------- special symbols ---------- */
        if(c=='{' || c=='}' || c=='(' || c==')' ||
           c==';' || c==',' )
        {
            t.row=row;
            t.col=col;
            t.name[0]=c;
            t.name[1]='\0';
            strcpy(t.type,"symbol");
            col++;
            return t;
        }

        if(c==' ' || c=='\t')
        {
            col++;
            continue;
        }
    }

    strcpy(t.type,"EOF");
    return t;
}

/* ---------- main ---------- */
int main()
{
    FILE *input;
    char fname[100];
    token t;
    int currentOutputRow = 0;

    printf("Enter filename: ");
    scanf("%s",fname);

    input=fopen(fname,"r");
    if(!input){
        printf("Cannot open file\n");
        return 1;
    }

    while(1)
    {
        t = getNextToken(input);
        if(strcmp(t.type,"EOF")==0)
            break;

        if(currentOutputRow != t.row)
        {
            if(currentOutputRow != 0)
                printf("\n");
            currentOutputRow = t.row;
        }

        printf("<%s,%d,%d> ", t.name, t.row, t.col);
    }

    fclose(input);
    return 0;
}
