#include<stdio.h>
#include<stdlib.h>
#define size 1024

int main(){
    FILE *file1, *file2;
    FILE *destfile;

    char f1[size], f2[size], dfile[size];
    printf("enter file1, file2 and destfile\n");
    scanf("%s%s%s", f1,f2,dfile);

    file1=fopen(f1, "r");
    file2=fopen(f2, "r");
    destfile=fopen(dfile, "w+");

    if(file1 == NULL || file2 == NULL || destfile == NULL)
        printf("error in file pointers\n");

    else
    {
        char line1[size], line2[size];
        while(fgets(line1, size, file1) !=NULL && fgets(line2, size, file2) !=NULL )
        {
            fputs(line1, destfile);
            fputs(line2, destfile);
        }
        while(fgets(line1, size, file1) !=NULL) 
            fputs(line1, destfile);
        while(fgets(line2, size, file2) !=NULL) 
            fputs(line2, destfile);

        fclose(file1);
        fclose(file2);
        fclose(destfile);
    }
    return 0;
}

