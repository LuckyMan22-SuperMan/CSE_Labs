#include<stdio.h>
#include<mpi.h>

void reverse(int n)
{
	int rev=0,rem;
	while(n>0)
	{
		rem=n%10;
		rev=rev*10+rem;
		n/=10;
	}
	
	printf("%d ",rev);
}

int main(int argc, char * argv[])
{
	MPI_Init(&argc,&argv);
	
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	int a[9]={18,523,123,12,34,54,789,56,527};
	
	reverse(a[rank]);
	
}
