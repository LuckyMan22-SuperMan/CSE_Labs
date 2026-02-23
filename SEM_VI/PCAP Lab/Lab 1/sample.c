#include<mpi.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
	int rank,size;
	
	MPI_Init(&argc,&argv);
	MPI_Status st;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	//printf("Processor %d of %d: Hello world\n",rank,size);
	int an=6;
	int f=0;

	if(rank==0)
	{
		MPI_Ssend(&an,1,MPI_INT,1,0,MPI_COMM_WORLD);
		printf("alswand\n");
	}
	if(rank==1)
	{
		printf("\nrecevied %d",f);
		MPI_Recv(&f,1,MPI_INT,0,0,MPI_COMM_WORLD,&st);
		
		
		
	}
	
	MPI_Finalize();
	
	return 0;
 
}
