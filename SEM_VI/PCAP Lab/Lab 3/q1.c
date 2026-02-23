#include<stdio.h>
#include<mpi.h>
int c;
int fact(int n)
{	
	c=1;
	while(n>0)
	{
		c*=n;
		n-=1;
	}
	return c;
}
int main(int argc, char * argv[])
{
	MPI_Init(&argc,&argv);
	int size,rank,n,i,a[10],b[10],f,res,m[10];

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	if(rank==0)
	{
		printf("Enter %d elements: \n",size);
		for(i=0;i<size;i++)
			scanf("%d",&a[i]);
	}
	
	MPI_Scatter(a,1,MPI_INT,&f,1,MPI_INT,0,MPI_COMM_WORLD);
	
	res=fact(f);
	MPI_Gather(&res, 1,MPI_INT,m,1,MPI_INT,0,MPI_COMM_WORLD);
	
	if(rank==0){
	
	
	for(i=0;i<size;i++)
		printf("%d ",m[i]);
	}
	
	MPI_Finalize();
	return 0;
	
	
}