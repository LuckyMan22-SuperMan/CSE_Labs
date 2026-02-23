#include<stdio.h>
#include<stdlib.h>


int main(){

FILE *fptr;
char filename[100];
int c_count=0, l_count=0;
printf("enter file name: \n");
scanf("%s", filename);

fptr= fopen(filename, "r");
if(fptr == NULL) 
    printf("error in fptr\n");

else
{
    int c = fgetc(fptr);
    char line[1024];
    while(c!=EOF)
    {
        c_count++;
        c=fgetc(fptr);
        if(c=='\n')
            l_count++;
    }

    printf("char count = %d and line count = %d\n", c_count, l_count);
    fclose(fptr);
    
}
return 0;

}