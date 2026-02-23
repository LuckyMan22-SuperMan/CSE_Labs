#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<unistd.h>

char keywords[32][10] = {
    "auto", "break", "case", "char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register", "return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

int checkKW(char word[])
{
    for(int i=0;i<32;i++)
        if(strcmp(keywords[i], word)==0)
            return 1;
    return 0;
}


int main()
{
    FILE *fp,*fp1;
    char src[100];int c,cp;
    char stripped[100]="mod.c";
    printf("\nEnter name of source file: ");
    scanf("%s", src);

    fp1 = fopen(src, "r");
    fp=fopen(stripped,"w+");

    if(fp1 == NULL)
    {
        printf("\nError opening file exiting");
        exit(1);
    }

    cp=fgetc(fp1);
	
	while(cp!=EOF)
	{
		if(cp=='#')
			while(cp!='\n')
				cp=fgetc(fp1);
		fputc(cp,fp);
		cp=fgetc(fp1);
	}
    fclose(fp1);
    fclose(fp);

    sleep(1);

    fp=fopen(stripped,"r");


//removed the header files
    char buf[100];
    int k = 0, i = 0;
    int row = 1, col = 1;

    c = fgetc(fp);

    while(c != EOF)
    {
        i = 0;
        buf[i] = '\0';
        k = 0;

        int start_col = col; 

        if(c == '\n')
        {
            row++;
            col = 0;
            printf("\n");
        }

        if(c=='=' || c=='+' || c=='-' || c=='*' || c=='/' || c==',' || c=='{' || c=='}' || c==';' || c=='(' || c==')')
        {
            buf[i++] = c;
            buf[i] = '\0';
            printf("<%s,%d,%d> ", buf, row, start_col);
        }

        if(isdigit(c))
        {
            buf[i++] = c;
            c = fgetc(fp);
            col++;

            while(c >= '0' && c <= '9')
            {
                buf[i++] = c;
                c = fgetc(fp);
                col++;
            }

            buf[i] = '\0';
            printf("<num,%d,%d> ", row, start_col);
            ungetc(c, fp);
            col--;
        }

        while(isalnum(c))
        {
            buf[i++] = c;
            c = fgetc(fp);
            k = 1;
            col++;
        }

        if(k == 1)
        {
            buf[i] = '\0';
            if(checkKW(buf))
                printf("<%s,%d,%d> ", buf, row, start_col);
            else
                printf("<id,%d,%d> ", row, start_col);

            ungetc(c, fp);
            col--;
        }

        c = fgetc(fp);
        col++;
    }

    fclose(fp);
    return 0;
}