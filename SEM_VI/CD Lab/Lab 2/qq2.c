#include<stdio.h>
#include<stdlib.h>

int main()
{
	FILE *f1,*f2;
	char s[100],d[100];
	printf("Enter name of src and dest file: ");
	scanf("%s%s",s,d);
	
	f1=fopen(s,"r");
	f2=fopen(d,"w+");
	
	char c=fgetc(f1);
	
	while(c!=EOF)
	{
		if(c=='#')
			while(c!='\n')
				c=fgetc(f1);
		fputc(c,f2);
		c=fgetc(f1);
	}
	
	fclose(f1);
	fclose(f2);
	
	return 0;
}
