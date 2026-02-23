#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_EOF,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING_LITERAL,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL
} TokenType;

typedef struct {
    int row;
    int col;
    TokenType type;
    char lexeme[100];
} Token;

FILE *fp;
int currentRow = 1;
int currentCol = 0;

const char *keywords[] = {
    "int", "float", "char", "if", "else",
    "while", "return", "void", "main", "for"
};
int kwCount = 10;

char getChar() {
    char c = fgetc(fp);

    if (c == '\n') {
        currentRow++;
        currentCol = 0;
    } else {
        currentCol++;
    }

    return c;
}

void unGetChar(char c) {
    if (c == EOF)
        return;

    if (c == '\n') {
        currentRow--;
        currentCol = 0;
    } else {
        currentCol--;
    }

    ungetc(c, fp);
}

int isKeyword(char *str) {
    for (int i = 0; i < kwCount; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

Token getNextToken() {
    Token token;
    token.lexeme[0] = '\0';
    token.type = TOKEN_EOF;

    char c;

    while ((c = getChar()) != EOF) {
        if (isspace(c)) {
            continue;
        }

        if (c == '#') {
            while ((c = getChar()) != '\n' && c != EOF);
            continue;
        }

        if (c == '/') {
            char next = getChar();

            if (next == '/') {
                while ((c = getChar()) != '\n' && c != EOF);
                continue;
            }
            else if (next == '*') {
                char prev = 0;
                while ((c = getChar()) != EOF) {
                    if (prev == '*' && c == '/')
                        break;
                    prev = c;
                }
                continue;
            }
            else {
                unGetChar(next);

                token.row = currentRow;
                token.col = currentCol;
                token.type = TOKEN_OPERATOR;
                strcpy(token.lexeme, "/");

                return token;
            }
        }

        if (c == '"') {
            token.row = currentRow;
            token.col = currentCol;
            token.type = TOKEN_STRING_LITERAL;

            int i = 0;
            while ((c = getChar()) != '"' && c != EOF) {
                if (i < 99)
                    token.lexeme[i++] = c;
            }

            token.lexeme[i] = '\0';
            return token;
        }

        if (isalpha(c) || c == '_') {
            token.row = currentRow;
            token.col = currentCol;

            int i = 0;
            token.lexeme[i++] = c;

            while (isalnum(c = getChar()) || c == '_') {
                if (i < 99)
                    token.lexeme[i++] = c;
            }

            token.lexeme[i] = '\0';
            unGetChar(c);

            if (isKeyword(token.lexeme))
                token.type = TOKEN_KEYWORD;
            else
                token.type = TOKEN_IDENTIFIER;

            return token;
        }

        if (isdigit(c)) {
            token.row = currentRow;
            token.col = currentCol;
            token.type = TOKEN_NUMBER;

            int i = 0;
            token.lexeme[i++] = c;

            while (isdigit(c = getChar())) {
                if (i < 99)
                    token.lexeme[i++] = c;
            }

            token.lexeme[i] = '\0';
            unGetChar(c);

            return token;
        }

        token.row = currentRow;
        token.col = currentCol;
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';

        if (strchr("+-*%=<>!&|", c)) {
            token.type = TOKEN_OPERATOR;

            char next = getChar();
            if ((c == '=' && next == '=') ||
                (c == '!' && next == '=') ||
                (c == '<' && next == '=') ||
                (c == '>' && next == '=') ||
                (c == '&' && next == '&') ||
                (c == '|' && next == '|')) {

                token.lexeme[1] = next;
                token.lexeme[2] = '\0';
            } else {
                unGetChar(next);
            }
        } else {
            token.type = TOKEN_SYMBOL;
        }

        return token;
    }

    token.row = currentRow;
    token.col = currentCol;
    token.type = TOKEN_EOF;
    strcpy(token.lexeme, "EOF");

    return token;
}

const char* getTypeString(TokenType t) {
    switch (t) {
        case TOKEN_KEYWORD:        return "Keyword";
        case TOKEN_IDENTIFIER:     return "Identifier";
        case TOKEN_NUMBER:         return "Number";
        case TOKEN_OPERATOR:       return "Operator";
        case TOKEN_STRING_LITERAL: return "String Literal";
        case TOKEN_SYMBOL:         return "Symbol";
        case TOKEN_EOF:            return "EOF";
        default:                   return "Unknown";
    }
}

int main() {
    fp = fopen("input_code1.c", "r");
    if (!fp) {
        printf("Error: Create input_code.c first.\n");
        return 1;
    }

    printf("| %-5s | %-5s | %-15s | %-20s |\n",
           "Row", "Col", "Type", "Lexeme");

    printf("|-------|-------|-----------------|----------------------|\n");

    Token t;

    while (1) {
        t = getNextToken();
        if (t.type == TOKEN_EOF)
            break;

        printf("| %-5d | %-5d | %-15s | %-20s |\n",
               t.row, t.col,
               getTypeString(t.type),
               t.lexeme);
    }

    fclose(fp);
    return 0;
}
