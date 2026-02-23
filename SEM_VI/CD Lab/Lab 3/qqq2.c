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

void getNextToken(FILE *input)
{
    int row=1, col=1;
    char c;

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
            row++; col=1;
            printf("\n");
            continue;
        }

        /* ---------- skip string ---------- */
        if(c=='"')
        {
            c=fgetc(input);
            while(c!='"' && c!=EOF)
            {
                if(c=='\n'){ row++; col=1; }
                c=fgetc(input);
            }
            col++;
            continue;
        }

        /* ---------- comments ---------- */
        else if(c=='/')
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
                while((c=fgetc(input))!=EOF)
                {
                    if(c=='\n') row++;
                    if(prev=='*' && c=='/') break;
                    prev=c;
                }
                col=1;
                continue;
            }
            else
            {
                printf("</, %d, %d>", row, col);
                col++;
                ungetc(next,input);
                continue;
            }
        }

        /* ---------- numbers ---------- */
        if(isdigit(c))
        {
            int startCol = col;
            buf[i++] = c;
            col++;

            c=fgetc(input);
            while(isdigit(c))
            {
                buf[i++] = c;
                col++;
                c=fgetc(input);
            }

            buf[i]='\0';
            printf("<num, %d, %d>", row, startCol);
            ungetc(c,input);
            continue;
        }

        /* ---------- identifiers & keywords ---------- */
        if(isalpha(c) || c=='_')
        {
            int startCol = col;
            buf[i++] = c;
            col++;

            c=fgetc(input);
            while(isalnum(c) || c=='_')
            {
                buf[i++] = c;
                col++;
                c=fgetc(input);
            }

            buf[i]='\0';
            ungetc(c,input);

            if(checkKW(buf))
                printf("<%s, %d, %d>", buf, row, startCol);
            else
                printf("<id, %d, %d>", row, startCol);

            continue;
        }

        /* ---------- arithmetic + increment/decrement ---------- */
        if(c=='+' || c=='-' || c=='*')
        {
            char next=fgetc(input);

            if((c=='+' && next=='+') ||
               (c=='-' && next=='-'))
            {
                printf("<%c%c, %d, %d>", c, next, row, col);
                col+=2;
            }
            else
            {
                printf("<%c, %d, %d>", c, row, col);
                col++;
                ungetc(next,input);
            }
            continue;
        }

        /* ---------- relational operators ---------- */
        if(c=='<' || c=='>' || c=='=' || c=='!')
        {
            char next=fgetc(input);

            if(next=='=')
            {
                printf("<%c%c, %d, %d>", c, next, row, col);
                col+=2;
            }
            else
            {
                printf("<%c, %d, %d>", c, row, col);
                col++;
                ungetc(next,input);
            }
            continue;
        }

        /* ---------- logical operators ---------- */
        if(c=='&' || c=='|')
        {
            char next=fgetc(input);

            if(next==c)
            {
                printf("<%c%c, %d, %d>", c, next, row, col);
                col+=2;
            }
            else
            {
                printf("<%c, %d, %d>", c, row, col);
                col++;
                ungetc(next,input);
            }
            continue;
        }

        /* ---------- special symbols ---------- */
        if(c=='{' || c=='}' || c=='(' || c==')' ||
           c==';' || c==',' )
        {
            printf("<%c, %d, %d>", c, row, col);
            col++;
            continue;
        }

        /* ---------- spaces ---------- */
        if(c==' ' || c=='\t')
        {
            col++;
            continue;
        }
    }
}

int main()
{
    FILE *input;
    char fname[100];

    printf("Enter filename: ");
    scanf("%s",fname);

    input=fopen(fname,"r");
    if(!input)
    {
        printf("Cannot open file\n");
        return 1;
    }

    getNextToken(input);
    fclose(input);
    return 0;
}
