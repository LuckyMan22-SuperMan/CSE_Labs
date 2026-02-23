#include<stdio.h>
#include<mpi.h>
#include<math.h>
int main(int argc, char *argv[])
{
	int x=2,n,rank,size;
	
	MPI_Init(&argc,&argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	

	if(rank%2==0)
		printf("Hello \n");
	else
		printf("World \n");
			
	
	printf("result is= %.0f rank is %d\n",pow(x,rank),rank);
	MPI_Finalize();
	return 0;
}
