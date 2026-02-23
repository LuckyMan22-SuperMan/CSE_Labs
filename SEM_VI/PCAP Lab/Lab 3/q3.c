#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include<unistd.h>
int main(int argc, char * argv[])
{
	MPI_Init(&argc, &argv);
	int size,rank,i,n=0,ch,dd;
	int rec[100];
	char a[100],b[100];
	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	if(rank==0)
	{
		printf("\nEnter the string: \n");
		gets(a);
		while((strlen(a)/size)%2 !=0)
		{
			printf("\nEnter correct length of string: ");
			gets(a);
		}
		ch=strlen(a)/size;
	}
	MPI_Bcast(&ch,1,MPI_INT,0,MPI_COMM_WORLD);
	
	MPI_Scatter(a,ch,MPI_CHAR,b,ch,MPI_CHAR,0,MPI_COMM_WORLD);
	
	b[ch]='\0';
	dd=0;
	for(i=0;i<ch;i++)
	{
		if(b[i]!='a' && b[i]!='e' && b[i]!='i' && b[i]!='o' && b[i]!='u')
			dd++;
	}
	
	printf("\nProcess %d received %s ",rank,b);
	
	MPI_Gather(&dd,1,MPI_INT, rec,1,MPI_INT,0,MPI_COMM_WORLD);
	
	if(rank==0)
	{
		sleep(1);
		for(i=0;i<size;i++)
		{
			printf("\nProcess %d found %d non-vowels",i,rec[i]);
			n+=rec[i];
		}
		printf("\nTotal non vowels are= %d",n);
		
	}
	
	MPI_Finalize();
	return 0;
	
}