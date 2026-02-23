#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKEN_LEN 100


bool isArithmeticOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == '%');
}

bool isRelationalOperator(char ch) {
    return (ch == '>' || ch == '<' || ch == '=');
}

bool isSpecialSymbol(char ch) {
    return (ch == ',' || ch == ';' || ch == '(' || ch == ')' ||
            ch == '[' || ch == ']' || ch == '{' || ch == '}');
}

bool isDelimiter(char ch) {
    return (isspace(ch) ||
            isArithmeticOperator(ch) ||
            isRelationalOperator(ch) ||
            isSpecialSymbol(ch));
}

bool isKeyword(char *str) {
    const char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue",
        "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "int", "long",
        "register", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
    };

    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }

    return false;
}

bool isInteger(char *str) {
    int len = strlen(str);
    if (len == 0)
        return false;

    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

bool isValidIdentifier(char *str) {
    if (!isalpha(str[0]) && str[0] != '_')
        return false;

    for (int i = 1; i < strlen(str); i++) {
        if (!isalnum(str[i]) && str[i] != '_')
            return false;
    }

    return true;
}

void analyzeToken(char *str) {
    if (isKeyword(str)) {
        printf("Keyword : %s\n", str);
    }
    else if (isInteger(str)) {
        printf("Numerical Constant : %s\n", str);
    }
    else if (isValidIdentifier(str)) {
        printf("Identifier : %s\n", str);
    }
    else {
        printf("Unknown Token : %s\n", str);
    }
}


int main() {
    char ch;
    char buffer[MAX_TOKEN_LEN];
    int bufIndex = 0;

    FILE *fp = fopen("input_code.c", "r");
    if (fp == NULL) {
        printf("Error: Could not open input_code.c. Please create the file first.\n");
        return 1;
    }

    printf("--- Lexical Analysis Output ---\n");

    while ((ch = fgetc(fp)) != EOF) {
        if (isDelimiter(ch)) {
            if (bufIndex != 0) {
                buffer[bufIndex] = '\0';
                analyzeToken(buffer);
                bufIndex = 0;
            }

            if (isArithmeticOperator(ch))
                printf("Operator (Arithmetic) : %c\n", ch);
            else if (isRelationalOperator(ch))
                printf("Operator (Relational) : %c\n", ch);
            else if (isSpecialSymbol(ch))
                printf("Special Symbol : %c\n", ch);
        }
        else if (ch == '"') {
            if (bufIndex != 0) {
                buffer[bufIndex] = '\0';
                analyzeToken(buffer);
                bufIndex = 0;
            }

            printf("String Literal : \"");
            while ((ch = fgetc(fp)) != EOF && ch != '"') {
                printf("%c", ch);
            }
            printf("\"\n");
        }
        else if (!isspace(ch)) {
            if (bufIndex < MAX_TOKEN_LEN - 1) {
                buffer[bufIndex++] = ch;
            }
        }
    }

    fclose(fp);
    return 0;
}
