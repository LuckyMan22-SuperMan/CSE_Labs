#include <stdio.h>
#include <string.h>

char table[10][20]; 


int hash(char *str)
{
    int sum = 0;
    for (int i=0;str[i]!='\0';i++)
        sum+=str[i];
    return sum%10;
}


int search(char *key)
{
    int index=hash(key);

    if (strcmp(table[index],key)==0)
        return index;

    return -1;
}

void insert(char *str)
{
    int index=hash(str);

    if (search(str)!=-1)
        printf("Verb %s already exists at index %d\n", str, index);
    else
    {
        strcpy(table[index],str);
        printf("Inserted verb %s at index %d\n", str, index);
    }
}


int main()
{
    int ch;
    char word[20];

    for (int i=0;i<10;i++)
        table[i][0]='\0';

    while (1)
    {
        printf("\n1.Insert verb\n 2.Search\n 3.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
            case 1:
                printf("Enter verb to insert: ");
                scanf("%s", word);
                insert(word);
                break;

            case 2:
                printf("Enter verb to search: ");
                scanf("%s", word);
                if (search(word) != -1)
                    printf("Verb found at index %d\n", search(word));
                else
                    printf("Verb not found\n");
                break;

            case 3:
                return 0;

            default:
                printf("Invalid ch haha lol\n");
        }
    }
}
