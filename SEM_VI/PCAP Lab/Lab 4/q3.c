// #include<stdio.h>
// #include<mpi.h>
// #include<unistd.h>
// int main(int argc, char * argv[])
// {
// 	MPI_Init(&argc, &argv);
// 	int rank,size;
	
// 	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
// 	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
// 	int i,j,a[4][4],key,ans=0,c=0;
// 	int b[4];
// 	int res[4];
	
// 	if(rank==0)
// 	{
// 		printf("Enter 4 x 4 matrix elements: \n");
// 		for(i=0;i<4;i++)
// 			for(j=0;j<4;j++)
// 				scanf("%d",&a[i][j]);
		
// 		printf("Your ans is:\n");
// 	}
// 	sleep(1);
// 	MPI_Scatter(a,4,MPI_INT,b,4,MPI_INT,0,MPI_COMM_WORLD);
	
// 	MPI_Scan(b,res,4,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	
// 	for(i=0;i<4;i++)
// 		printf("%d ",res[i]);
// 	printf("\n");
	
// 	MPI_Finalize();
// 	return 0;
// }

#include<stdio.h>
#include<mpi.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char * argv[])
{
	MPI_Init(&argc,&argv);
	int size,rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	int a[4][4],b[4],c[4],i,j,sum;

	if(rank==0)
	{
		printf("Enter 4 x 4 elemnts: \n");
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				scanf("%d",&a[i][j]);
		printf("Your ans is: ");
	}
	sleep(1);
	MPI_Scatter(a,4,MPI_INT,b,4,MPI_INT,0,MPI_COMM_WORLD);

	MPI_Scan(b,c,4,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	for(int p=0;p<size;p++)
	{
		//MPI_Barrier(MPI_COMM_WORLD);
		if(rank==p)
			{
				for(i=0;i<4;i++)
					printf("%d ",c[i]);
				
				printf("\n");
			}
	}
	
	MPI_Finalize();
	return 0;
}
