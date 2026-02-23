#include<stdio.h>
#include<stdlib.h>

int main(){

    FILE *fptr;
    char filename[1024];
    printf("enter file name: \n");
    scanf("%s",filename);
    fptr=fopen(filename, "r");

    if(fptr == NULL) 
        printf("error in fptr\n");
    
    else
    {
        fseek(fptr, 0 ,SEEK_END);   //move file pointer to the end (required for ftell to work)
        int size= ftell(fptr);
        printf("size of file: %d\n", size);

        char destfile[1024];    
        printf("enter dest file:");
        scanf("%s",destfile);

        FILE* dptr=fopen(destfile, "w+");
        for(int i=size-1; i>=0;i--)
        {
            fseek(fptr, i , SEEK_SET);
            char c = fgetc(fptr);
            fputc(c,dptr);
        }
        fclose(dptr);
        fclose(fptr);
    }
    return 0;

}