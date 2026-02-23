#include<stdio.h>
#include<ctype.h>
#include<string.h>

typedef struct{
    char name[1024];
    char type[1024];
    int row, col;
}token;

int col= 1, row =1;

char keyword[32][10] = 
{
    "auto","break","case","char","const","continue","default","do",
 "double","else","enum","extern","float","for","goto","if",
 "int","long","register","return","short","signed","sizeof","static",
 "struct","switch","typedef","union","unsigned","void","volatile","while"
};

int chKeyword(char str[])
{
    for(int i=0;i<32;i++)
    {
        if(strcmp(keyword[i], str)==0) return 1;
    }
    return 0;
}

void skipPreprocess(FILE*fp)
{
    char c = fgetc(fp);
    while(c != '\n' && c!=EOF)
    {
        c = fgetc(fp);
    }
    row++;
    col= 1;
}

void skipComments(char s, FILE*fp)
{
    char c = fgetc(fp), prev=0;

    if(c=='/')
    {
        while(c!='\n' && c!= EOF)
        {
            c = fgetc(fp);
        }
        row++;
        col= 1; 
    }
    else if(c == '*')
    {
            while(c != EOF)
            {
                if(c == '\n')
                {
                    row++; 
                    col=1;
                }
                else if(prev == '*' && c == '/')
                    break;

                prev = c;
                c=fgetc(fp);
            }
        }
    else 
        c = ungetc(c , fp);

}
token getNextToken(FILE*fp)
{
    char buf[1024];
    token t;
    t.name[0] = '\0';
    t.type[0]='\0';
    while(1)
    {
        
        char c = fgetc(fp);
        if(isspace(c))
        {
            if(c == '\n'){ row++; col=1;}
            else{
                col++;
            }
            continue;
        }
        if(c == EOF)
        {
            strcpy(t.type, "EOF");
            return t;
        }
        if(c == '#')
        {
            skipPreprocess(fp);continue;
        }
        if(c == '/')
        {
            skipComments(c, fp);continue;
        }

        t.col=col;
        t.row=row;

        if(c == '"')
        {
            int i=0;
            c = fgetc(fp);
            while(c!=EOF && c!='"')
            {
                buf[i++]=c;
                c = fgetc(fp);
            }
            buf[i]='\0';
            col += strlen(buf)+2;
            strcpy(t.name, buf);
            strcpy(t.type, "STRING_LITERAL");
            return t;
        }
        if(isdigit(c))
        {
            int i=1;
            buf[0]=c;
            c = fgetc(fp);
            while(c!=EOF && isdigit(c))
            {
                buf[i++]=c;
                c = fgetc(fp);

            }
            buf[i]='\0'; 
            c = ungetc(c, fp); 
            col+=strlen(buf);
        
            strcpy(t.name, buf);
            strcpy(t.type, "NUMBER");
            return t;
        }

        if(isalnum(c) || c == '_')
        {
        int i=1;
        buf[0]=c;
            c = fgetc(fp);
            while(c!=EOF && (isalnum(c) || c == '_'))
            {
                buf[i++]=c;
                c = fgetc(fp);
            }

            buf[i]='\0';
            c = ungetc(c, fp); 
            col+=strlen(buf);
            strcpy(t.name, buf);
            if(chKeyword(buf)) 
                strcpy(t.type, "KEYWORD");
            else 
                strcpy(t.type, "IDENTIFIER");
            return t;
        }
        
        col++;
        if (c == '=' || c == '!' || c == '<' || c == '>' || c == '&' || c == '|')
        {
            char d = fgetc(fp);
            if ((c == '&' && d == '&') || (c == '|' && d == '|'))
            {
                t.name[0] = c;
                t.name[1] = d;
                t.name[2] = '\0';
                col++;
                strcpy(t.type, "LogicalOperator");
            }
            else if (d == '=')
            {
                t.name[0] = c;
                t.name[1] = d;
                t.name[2] = '\0';
                if (c == '=' || c == '!' || c == '<' || c == '>')
                    strcpy(t.type, "RelationalOperator");
                else
                    strcpy(t.type, "Operator");
                col++;
            }
            else if (c == '!')
            {
                t.name[0] = c;
                t.name[1] = '\0';
                strcpy(t.type, "LogicalOperator");
                ungetc(d, fp);
            }
            else if (c == '<' || c == '>')
            {
                t.name[0] = c;
                t.name[1] = '\0';
                strcpy(t.type, "RelationalOperator");
                ungetc(d, fp);
            }
            else if (c == '=')
            {
                t.name[0] = c;
                t.name[1] = '\0';
                strcpy(t.type, "AssignmentOperator");
                ungetc(d, fp);
            }
            return t;
        }

        if(c== '+' || c=='-' || c=='*' || c=='/' || c=='%' || c=='^')
        {
            char d = fgetc(fp);
            if(d == '=')
            {
                col++;
                t.name[0]=c;
                t.name[1]=d;
                t.name[2]='\0';
                strcpy(t.type, "ASSIGNMENT OP");
                return t;
            }
            else if(d == '+' || d == '-')
            {
                col++;
                t.name[0]=c;
                t.name[1]=d;
                t.name[2]='\0';
                strcpy(t.type, "ARITHMNETIC OP");
                return t;
            }
            else 
                d = ungetc(d, fp);

            t.name[0]=c;
            t.name[1]='\0';
            strcpy(t.type, "ARITHMNETIC OP");
            return t;
        }
    }
}

int main(){
    FILE *fp;
    char filename[1024];
    printf("enter file name\n");
    gets(filename);
    fp = fopen(filename, "r");
    if(!fp) 
        return -1;
    while(1)
        {
        token t = getNextToken(fp);
        if(strcmp(t.type, "EOF")!=0)
        {
            printf("<%s, %s, %d, %d>\n", t.name, t.type, t.row, t.col);
        }
        else break;
    }
}