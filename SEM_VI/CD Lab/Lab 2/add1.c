#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int isDataType(char word[])
{
    char types[][10] = {
        "int","float","double","char","void",
        "long","short","signed","unsigned"
    };

    for(int i=0;i<9;i++)
        if(strcmp(types[i],word)==0)
            return 1;

    return 0;
}

int main()
{
    FILE *fp;
    char fname[100];

    printf("Enter file name: ");
    scanf("%s", fname);

    fp = fopen(fname,"r");
    if(fp==NULL)
    {
        printf("Error opening file");
        exit(1);
    }

    int c;
    char word[100], prev[100];
    int i = 0;

    while((c = fgetc(fp)) != EOF)
    {
        if(isalnum(c) || c=='_')
        {
            word[i++] = c;
        }
        else
        {
            if(i > 0)
            {
                word[i] = '\0';
                strcpy(prev, word);
                i = 0;
            }

            /* check function */
            if(c == '(' && isDataType(prev))
            {
                char fname[100];
                int j = 0;

                /* read function name */
                while((c = fgetc(fp)) != '(' && c!=EOF)
                {
                    if(isalnum(c) || c=='_')
                        fname[j++] = c;
                }

                fname[j] = '\0';

                /* count arguments */
                int args = 0;
                int flag = 0;

                while(c != ')' && c != EOF)
                {
                    c = fgetc(fp);

                    if(c == ',')
                        args++;

                    if(isalnum(c))
                        flag = 1;
                }

                if(flag)
                    args++;

                printf("\nReturn Type: %s", prev);
                printf("\nFunction Name: %s", fname);
                printf("\nArguments: %d\n", args);
            }
        }
    }

    fclose(fp);
    return 0;
}
