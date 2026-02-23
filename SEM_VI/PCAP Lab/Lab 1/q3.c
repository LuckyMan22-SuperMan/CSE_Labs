#include<mpi.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char * argv[])
{
	int size,rank,n;
	char str[]="HELLO";
	MPI_Init(&argc,&argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	if(str[rank]>='a' && str[rank]<='z')
		str[rank]-=32;
		
	if(str[rank]>='A' && str[rank]<='Z')
		str[rank]+=32;
	printf("Process with rank %d modifies to: ",rank); puts(str);
	
	MPI_Finalize();
}
