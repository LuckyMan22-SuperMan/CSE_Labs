#include<stdio.h>
#include<mpi.h>

int fib(int n)
{
	if(n==0 || n==1)
		return n;
	return fib(n-1) + fib(n-2);
}

void factorial(int n)
{
	int ans=1;
	int t=n;
	while(n>0)
	{
		ans=ans*n;
		n-=1;
	}
	
	printf("factorial of rank %d is %d\n",t,ans);
	
	
}
int main(int argc, char * argv[])
{
	int size,rank,g;
	MPI_Init(&argc,&argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	if(rank%2==0)
		factorial(rank);
	else
		{
			g=fib(rank);
			printf("fibo is of rank %d is %d\n",rank,g);
		}
}
