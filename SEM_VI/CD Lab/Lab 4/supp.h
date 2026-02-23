#include<stdio.h>
#include<string.h>
#include<ctype.h>

typedef struct{
    char name[1024];
    char type[1024];
    int row, col;
}token;

char keywords[3][100] = 
{
    "let", "document",  "function"
};
char func[9][100] = 
{
    "ready", "on", "val", "split", "map", "filter", "text", "join", "isNaN"
};


int checkFunc(char *str)
{
    for(int i=0;i<9;i++)
    {
        if(strcmp(func[i], str)==0) 
            return 1;

    }
    return 0;
}

int checkKW(char*str)
{
    for(int i=0;i<3;i++)
    {
        if(strcmp(keywords[i], str)==0) 
            return 1;

    }
    return 0;
}

int row = 1, col=1;

token getNextToken(FILE *fp)
{
    char buf[1024];
    token tk;
    tk.name[0]='\0';
    tk.type[0]='\0';
    while(1)
    {
        char c = fgetc(fp);
        if(isspace(c))
        {
            if(c == '\n')
            {
                row++; 
                col=1;
            }
            else 
                col++;
            continue;
        }
        if(c==EOF)
        {
            strcpy(tk.type, "EOF");
            return tk;
        }
        tk.row= row;
        tk.col = col;
        if(isdigit(c))
        {
            int i=0;
            while(c!=EOF && isdigit(c))
            {
                buf[i++]=c;
                c = fgetc(fp);
            }
            buf[i]='\0';
            col+=strlen(buf);
            c = ungetc(c, fp);
            strcpy(tk.type, "NUMBER");
            strcpy(tk.name, buf);
            return tk;
        }
        if(c == '"')
        {
            c = fgetc(fp);
            int flag=1;
            if(c == '#') flag=0;
            int i=0;
            while(c!=EOF && c!='"')
            {
                buf[i++]=c;
                c = fgetc(fp);
            }
            buf[i]='\0';
            col+=strlen(buf)+2;
            if(!flag) strcpy(tk.type, "IDENTIFIER");
            else strcpy(tk.type, "STRING");
            strcpy(tk.name, buf);
            return tk;

        }

        if(isalnum(c))
        {
            int i=0;
            while(c!=EOF && (isalnum(c) || c=='_'))
            {
                buf[i++]=c;
                c = fgetc(fp);
            }
            buf[i]='\0';
            col+=strlen(buf);
            c = ungetc(c, fp);
            if(!checkKW(buf)) strcpy(tk.type, "IDENTIFIER");
            else strcpy(tk.type, "KEYWORD");
            strcpy(tk.name, buf);
            return tk;
        }
        if(c == '+' || c == '-' || c == '/' || c == '*' || c == '&' || c == '$' || c == '&' || c == '!' || c == ':' || c=='=' || c=='%')
        {
            char d = fgetc(fp);
            strcpy(tk.type, "OPERATOR");
            if((c == '=' && d == '=') || c == '&' && d == '&')
            {
                col++;
                buf[0]=c;
                buf[1]=d;
                buf[2]='\0';
            }
            else
            {
                buf[0]=c;
                buf[1]='\0';
                col++;
            }
            strcpy(tk.name, buf);
            return tk;
        }

        tk.name[0]=c;
        tk.name[1]='\0';
        strcpy(tk.type, "SPECIAL SYMBOL");
        col++;
        return tk;


    }
}
