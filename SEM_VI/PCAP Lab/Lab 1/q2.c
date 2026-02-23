#include<mpi.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
	MPI_Init(&argc,&argv);
	
	int rank,size,n;
	char op;
	
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	int a=8,b=2;
	
	int ans;
	
	//printf("haha %d",ans);
	
	switch(rank)
	{
	
	case 0:
		ans=a+b;
		printf("\nPerforming addition of %d and %d result= %d",a,b,ans);
		break;
		
	case 1:
		ans=a-b;
		printf("\nPerforming subtraction of %d and %d result= %d",a,b,ans);
		break;
	case 2:
		ans=a*b;
		printf("\nPerforming multiplication of %d and %d result= %d",a,b,ans);
		break;
	case 3:
		ans=a/b;
		printf("\nPerforming division of %d and %d result= %d\n",a,b,ans);
	}
	
}
