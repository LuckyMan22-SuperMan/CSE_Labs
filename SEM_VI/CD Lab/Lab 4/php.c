#include<stdio.h>
#include<string.h>
#include<ctype.h>

typedef struct
{
    char name[1024];
    char type[1024];
    int row, col;
} token;

typedef struct
{
    char name[1024];
    char type[1024];
    char arg[1024];
} table;

char *keywords[] =
{
    "int", "float", "char", "if", "else", "while", "return",
    "for", "do", "switch", "case", "break", "continue",
    "void", "struct", "double", "long", "short", "sizeof",
    "typedef", "const", "exit", NULL
};

int isKeyword(char *str)
{
    for (int i = 0; keywords[i] != NULL; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

table tb[1024];
int ind = 0;
int row = 1, col = 1;

void insert(token tk, char *arg)
{
    for (int i = 0; i < ind; i++)
    {
        if (strcmp(tk.name, tb[i].name) == 0)
            return;
    }

    strcpy(tb[ind].name, tk.name);
    strcpy(tb[ind].type, tk.type);
    strcpy(tb[ind].arg, arg);
    ind++;
}

void display()
{
    printf("Token Name\t\tArguments\n");
    for (int i = 0; i < ind; i++)
    {
        printf("%s\t\t%s\n", tb[i].name, tb[i].arg);
    }
}

void skipComments(FILE *fp)
{
    char c = fgetc(fp), prev = 0;

    if (c == '/')
    {
        while (c != EOF && c != '\n')
            c = fgetc(fp);

        col = 1;
        row++;
    }
    else if (c == '*')
    {
        while (c != EOF)
        {
            if (c == '\n')
            {
                row++;
                col = 1;
            }
            else if (prev == '*' && c == '/')
                break;

            prev = c;
            c = fgetc(fp);
        }
    }
    else
        ungetc(c, fp);
}

void skipPP(FILE *fp)
{
    char c = fgetc(fp);

    while (c != EOF && c != '\n')
        c = fgetc(fp);

    row++;
    col = 1;
}

token getNextToken(FILE *fp)
{
    char buf[1024];
    token tk;

    while (1)
    {
        char c = fgetc(fp);

        if (isspace(c))
        {
            if (c == '\n')
                row++;
            else
                col++;

            continue;
        }

        if (c == EOF)
        {
            strcpy(tk.type, "EOF");
            return tk;
        }

        if (c == '#')
        {
            skipPP(fp);
            continue;
        }

        if (c == '/')
        {
            skipComments(fp);
            continue;
        }

        tk.row = row;
        tk.col = col;

        if (c == '"')
        {
            int i = 0;
            c = fgetc(fp);

            while (c != EOF && c != '"')
            {
                buf[i++] = c;
                c = fgetc(fp);
            }

            buf[i] = '\0';
            col += strlen(buf) + 2;

            strcpy(tk.type, "STRING");
            strcpy(tk.name, buf);
            return tk;
        }

        if (isdigit(c))
        {
            int i = 0;

            while (c != EOF && isdigit(c))
            {
                buf[i++] = c;
                c = fgetc(fp);
            }

            buf[i] = '\0';
            col += strlen(buf);

            ungetc(c, fp);

            strcpy(tk.type, "NUMBER");
            strcpy(tk.name, buf);
            return tk;
        }

        if (isalnum(c) || c == '$')
        {
            int i = 0;

            while (c != EOF && (isalnum(c) || c == '$'))
            {
                buf[i++] = c;
                c = fgetc(fp);
            }

            buf[i] = '\0';
            col += strlen(buf);

            ungetc(c, fp);

            if (buf[0] == '$' || isKeyword(buf))
                strcpy(tk.type, "IDENTIFIER");
            else
                strcpy(tk.type, "FUNCTION");

            strcpy(tk.name, buf);
            return tk;
        }

        if (c == '+' || c == '-' || c == '/' || c == '*')
        {
            char d = fgetc(fp);

            strcpy(tk.type, "ARITHMETIC OPERATOR");

            if (d == '+' || d == '-')
            {
                buf[0] = c;
                buf[1] = d;
                buf[2] = '\0';
                col++;
            }
            else
            {
                buf[0] = c;
                buf[1] = '\0';
                ungetc(d, fp);
            }

            strcpy(tk.name, buf);
            return tk;
        }

        if (c == '<' || c == '>' || c == '=' ||
            c == '&' || c == '|' || c == '!')
        {
            char d = fgetc(fp);

            if ((c == '&' && d == '&') ||
                (c == '|' && d == '|'))
            {
                buf[0] = c;
                buf[1] = d;
                buf[2] = '\0';
                col++;
                strcpy(tk.type, "LOGICAL OPERATOR");
            }
            else if (d == '=')
            {
                buf[0] = c;
                buf[1] = d;
                buf[2] = '\0';
                col++;
                strcpy(tk.type, "RELATIONAL OPERATOR");
            }
            else
            {
                buf[0] = c;
                buf[1] = '\0';
                ungetc(d, fp);

                if (c == '!')
                    strcpy(tk.type, "LOGICAL OPERATOR");
                else
                    strcpy(tk.type, "RELATIONAL OPERATOR");
            }

            strcpy(tk.name, buf);
            return tk;
        }

        tk.name[0] = c;
        tk.name[1] = '\0';
        col++;

        strcpy(tk.type, "SPECIAL SYM");
        return tk;
    }
}

int main()
{
    FILE *fp;
    char i_name[1024], o_file[1024];

    printf("enter input_file\n");
    gets(i_name);

    printf("enter output_file\n");
    gets(o_file);

    fp = fopen(i_name, "r");
    FILE *out = fopen(o_file, "w+");

    char c = fgetc(fp);

    while (c != EOF)
    {
        if (c == '/')
        {
            skipComments(fp);
            c = fgetc(fp);
            continue;
        }

        if (c == '<')
        {
            char d = fgetc(fp);

            if (d == '?')
            {
                while (c != EOF && c != '\n')
                    c = fgetc(fp);

                fgetc(fp);
            }
            else
            {
                fputc(c, out);
                c = fgetc(fp);
            }
        }
        else if (c == '?')
        {
            char d = fgetc(fp);

            if (d == '>')
                c = fgetc(fp);
        }
        else
        {
            fputc(c, out);
            c = fgetc(fp);
        }
    }

    rewind(out);

    while (1)
    {
        token tk = getNextToken(out);

        if (strcmp(tk.type, "EOF") == 0)
            break;

        if (strcmp(tk.type, "FUNCTION") == 0)
        {
            token next = getNextToken(out);
            char buf[1024] = "";

            if (strcmp(next.name, "(") == 0)
            {
                next = getNextToken(out);

                while (strcmp(next.name, ")") != 0)
                {
                    strcat(buf, next.name);
                    strcat(buf, " ");   // FIX: space between args
                    next = getNextToken(out);
                }
            }
            else if (strcmp(next.type, "STRING") == 0)
            {
                strcpy(buf, next.name);
            }

            insert(tk, buf);
        }
    }

    rewind(out);
    row = 1;
    col = 1;

    while (1)
    {
        token tk = getNextToken(out);

        if (strcmp(tk.type, "EOF") == 0)
            break;

        printf("<%s, %s, %d, %d>\n",
               tk.name, tk.type, tk.row, tk.col);
    }

    display();
}
