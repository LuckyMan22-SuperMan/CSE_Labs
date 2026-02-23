#include<stdio.h>
#include<stdlib.h>

int main()
{
	FILE *f1,*f2;
	char s[100],d[100];
	printf("Enter src and dest file name: ");
	scanf("%s%s",s,d);
	f1=fopen(s,"r");
	f2=fopen(d,"w+");
	
	if(f1==NULL || f2== NULL)
	{
		printf("Error in opening exiting");
		exit(1);
	}
	
	char c=fgetc(f1);
	
	while(c!=EOF)
	{
		if(c==' ' || c=='\t')
		{
			c=' ';
			fputc(c,f2);
			while(c==' ' || c=='\t')
				c=fgetc(f1);
		}
		
		else
		{
			fputc(c,f2);
			c=fgetc(f1);
		}
	}
	
	fclose(f1);
	fclose(f2);
	
	return 0;
}
