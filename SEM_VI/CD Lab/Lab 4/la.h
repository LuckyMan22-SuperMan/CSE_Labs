// The following C lexical analyzer handles keywords, identifiers, numbers, special symbols, arithmetic operators, relational operators, string literals, logical operators. It skips comments and preprocessor directives

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct token
{
    char lexeme[30];
    char type[30];
    int row, col;
};

int row = 1, col = 1;

char *keywords[] = 
{"int", "float", "char", "if", "else", "while", "return", 
    "for", "do", "switch", "case", "break", "continue", 
    "void", "struct", "double", "long", "short", "sizeof", "typedef", "const", "exit", NULL};

int isKeyword(char *str)
{
    for (int i = 0; keywords[i] != NULL; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

struct token getNextToken(FILE *fp)
{
    struct token tkn;
    tkn.lexeme[0] = '\0';
    tkn.type[0] = '\0';

    int c = fgetc(fp);

    // Skip whitespaces
    while (isspace(c))
    {
        if (c == '\n')
        {
            row++;
            col = 1;
        }
        else
            col++;
        c = fgetc(fp);
    }

    // EOF
    if (c == EOF)
    {
        strcpy(tkn.type, "EOF");
        return tkn;
    }

    // Skip preprocessor directives
    if (c == '#')
    {
        while ((c = fgetc(fp)) != EOF && c != '\n')
            ;
        row++;
        col = 1;
        return getNextToken(fp);
    }

    // Skip comments
    if (c == '/')
    {
        int d = fgetc(fp);
        if (d == '/')
        {
            while ((c = fgetc(fp)) != EOF && c != '\n')
                ;
            row++;
            col = 1;
            return getNextToken(fp);
        }
        else if (d == '*')
        {
            int prev = 0;
            while ((c = fgetc(fp)) != EOF)
            {
                if (c == '\n')
                {
                    row++;
                    col = 1;
                }
                else
                    col++;
                if (prev == '*' && c == '/')
                    break;
                prev = c;
            }
            col++;
            return getNextToken(fp);
        }
        else
            ungetc(d, fp);
    }

    // Save position for the actual token
    tkn.row = row;
    tkn.col = col;

    // String literals
    if (c == '"')
    {
        int i = 0;
        tkn.lexeme[i++] = '"';
        col++;
        while ((c = fgetc(fp)) != EOF && c != '"')
        {
            if (c == '\n')
            {
                row++;
                col = 1;
            }
            else
                col++;
            tkn.lexeme[i++] = c;
        }
        tkn.lexeme[i++] = '"';
        tkn.lexeme[i] = '\0';
        col++;
        strcpy(tkn.type, "StringLiteral");
        return tkn;
    }

    // Identifier or Keyword
    if (isalpha(c) || c == '_')
    {
        int i = 0;
        tkn.lexeme[i++] = c;
        col++;
        while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_'))
        {
            col++;
            tkn.lexeme[i++] = c;
        }
        tkn.lexeme[i] = '\0';
        ungetc(c, fp);
        if (isKeyword(tkn.lexeme))
            strcpy(tkn.type, "Keyword");
        else
            strcpy(tkn.type, "Identifier");
        return tkn;
    }

    // Numbers
    if (isdigit(c))
    {
        int i = 0;
        tkn.lexeme[i++] = c;
        col++;
        while ((c = fgetc(fp)) != EOF && isdigit(c))
        {
            col++;
            tkn.lexeme[i++] = c;
        }
        tkn.lexeme[i] = '\0';
        ungetc(c, fp);
        strcpy(tkn.type, "Number");
        return tkn;
    }

    // Relational, Logical, Assignment
    if (c == '=' || c == '!' || c == '<' || c == '>' || c == '&' || c == '|')
    {
        int d = fgetc(fp);
        col++;
        if ((c == '&' && d == '&') || (c == '|' && d == '|'))
        {
            tkn.lexeme[0] = c;
            tkn.lexeme[1] = d;
            tkn.lexeme[2] = '\0';
            col++;
            strcpy(tkn.type, "LogicalOperator");
        }
        else if (d == '=')
        {
            tkn.lexeme[0] = c;
            tkn.lexeme[1] = d;
            tkn.lexeme[2] = '\0';
            if (c == '=' || c == '!' || c == '<' || c == '>')
                strcpy(tkn.type, "RelationalOperator");
            else
                strcpy(tkn.type, "Operator");
            col++;
        }
        else if (c == '!')
        {
            tkn.lexeme[0] = c;
            tkn.lexeme[1] = '\0';
            strcpy(tkn.type, "LogicalOperator");
            ungetc(d, fp);
        }
        else if (c == '<' || c == '>')
        {
            tkn.lexeme[0] = c;
            tkn.lexeme[1] = '\0';
            strcpy(tkn.type, "RelationalOperator");
            ungetc(d, fp);
        }
        else if (c == '=')
        {
            tkn.lexeme[0] = c;
            tkn.lexeme[1] = '\0';
            strcpy(tkn.type, "AssignmentOperator");
            ungetc(d, fp);
        }
        else
        {
            tkn.lexeme[0] = c;
            tkn.lexeme[1] = '\0';
            strcpy(tkn.type, "Unknown");
            ungetc(d, fp);
        }
        return tkn;
    }

    // Arithmetic
    if (strchr("+-*/%", c))
    {
        tkn.lexeme[0] = c;
        tkn.lexeme[1] = '\0';
        strcpy(tkn.type, "ArithmeticOperator");
        col++;
        return tkn;
    }

    // Special Symbols
    if (strchr("()[]{},;:$#@'", c))
    {
        tkn.lexeme[0] = c;
        tkn.lexeme[1] = '\0';
        strcpy(tkn.type, "SpecialSymbol");
        col++;
        return tkn;
    }

    // Unknown character
    tkn.lexeme[0] = c;
    tkn.lexeme[1] = '\0';
    strcpy(tkn.type, "Unknown");
    col++;
    return tkn;
}