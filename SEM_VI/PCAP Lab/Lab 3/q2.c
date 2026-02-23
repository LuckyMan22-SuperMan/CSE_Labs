#include<stdio.h>
#include<mpi.h>
#include<unistd.h>
int main(int argc, char * argv[])
{
	int size,rank,i;
	int a[100],b[100];
	float ans[100];
	int m;
	int sum=0;
	
	MPI_Init(&argc,&argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	if(rank==0)
	{
		printf("Enter m: \n");
		scanf("%d",&m);

		printf("Enter %d*%d elemnts: \n",size,m);
		for(i=0;i<size*m;i++)
			scanf("%d",&a[i]);
		
	}
	MPI_Bcast(&m,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(a,m,MPI_INT,b,m,MPI_INT,0,MPI_COMM_WORLD);

	for(i=0;i<m;i++)
		sum+=b[i];
	float ad=(float)sum/m;
	MPI_Gather(&ad,1,MPI_FLOAT,ans,1,MPI_FLOAT,0,MPI_COMM_WORLD);

	if(rank==0)
	{
		float ff=0.0;
		for(i=0;i<size;i++)
		{
			printf("%f ",ans[i]);
			ff+=ans[i];
		}
		ff=ff/size;
		printf("Final avg is %f",ff);
	}
	MPI_Finalize();
}