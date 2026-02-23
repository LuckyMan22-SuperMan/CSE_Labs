#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

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
    {
        if(strcmp(keywords[i], word)==0) 
        	return 1;
    }
    return 0;
}

int isRel(char c)
{
    return (c=='=' || c=='<' || c=='>' || c=='!'); 
}

int isArith(char c)
{
    return (c=='+' || c=='-' || c=='*' || c=='/'); 
}

int isLog(char c)
{
    return (c=='&' || c=='|');
}

int isSpcl(char c)
{
    return (c=='{' || c=='}' || c=='@' || c=='$'); 
}

int isNum(char c)
{
    return (c>='0' && c<='9'); 
}

int isStr(char c)
{
    return (c=='"'); 
}

int isKworId(char c)
{
    return isalnum((unsigned char)c); 
}

int main()
{
	FILE *fp;
	char c, src[20];
	
	printf("Enter filename to enter: ");
	scanf("%s",src);
	fp=fopen(src,"r");
	if(fp==NULL)
	{
		printf("\nError openeing file exiting");
		exit(1);
	}

	c='a';
	char buf[100];
	int k=0,i=0;
	c=fgetc(fp);

    //printf("out");

	while(c!=EOF)
    {
        //printf("ha");
        i=0;
        buf[i]='\0';
        k=0;

        if(isRel(c))                         //== or = relarional
        {
            if(c=='=')
            {
                buf[i++]=c;
                c=fgetc(fp);

                if(c=='=')
                {
                    buf[i++]=c;
                    buf[i]='\0';
                    printf("\n%s is relational operator",buf);
                }
                else
                {
                    buf[i]='\0';
                    printf("\n%s is assignment operator",buf);
                }
            }
            else if(c=='<' || c=='>' || c=='!')
            {
                buf[i++]=c;
                c=fgetc(fp);

                if(c=='=')
                    buf[i++]=c;

                buf[i]='\0';
                printf("\n%s is relational operator",buf);
            }
        }
        else if(isArith(c))                     //Aritmertic and ++/-- arithmetic
        {
            buf[i++]=c;
            c=fgetc(fp);

            if(c=='+' || c=='-')
            {
                buf[i++]=c;
                buf[i]='\0';
                printf("\n%s is increment/decrement operator",buf);
            }
            else
            {
                buf[i]='\0';
                printf("\n%s is arithmetic operator",buf);
            }
        }
        else if(isLog(c))                      //logical
        {
            buf[i++]=c;
            c=fgetc(fp);
            if(c=='&' || c=='|')
            {
                buf[i++]=c;
                buf[i]='\0';
                printf("\n%s is logical operator",buf);
            }
            else
            {
                buf[i]='\0';
                printf("\n%s is bitwise operator",buf);
                //ungetc(c,fp);
            }
        }
        else if(isSpcl(c))                     //special
        {
            buf[i++]=c;
            buf[i]='\0';
            printf("\n%s is special character",buf);
        }
        else if(isNum(c))                       //nos.
        {
            buf[i++]=c;
            c=fgetc(fp);
            while(c>='0' && c<='9')
            {
                buf[i++]=c;
                c=fgetc(fp);
            }
            buf[i]='\0';
            printf("\n%s is numeric constant",buf);
        }
        else if(isStr(c))                       //strign
        {
            buf[i++]=c;
            c=fgetc(fp);
            while(c!='"')
            {
                buf[i++]=c;
                c=fgetc(fp);
            }
            buf[i++]=c;
            buf[i]='\0';
            printf("\n%s is string literal",buf);
        }

        while(isKworId(c))                          //keyword and id
        {
            buf[i++]=c;
            c=fgetc(fp);
            k=1;
        }
        if(k==1)
        {
            buf[i]='\0';
            if(checkKW(buf))
                printf("\n%s is keyword",buf);
            else
                printf("\n%s is identifier",buf);
            //ungetc(c,fp);
        }

        c=fgetc(fp);
	}
	
	fclose(fp);
	
	return 0;
	
}