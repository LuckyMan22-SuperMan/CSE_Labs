// #include<stdio.h>
// #include<mpi.h>
// #include<unistd.h>
// int main(int argc, char * argv[])
// {
// 	MPI_Init(&argc, &argv);
// 	int rank,size;
	
// 	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
// 	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
// 	int i,j,a[3][3],key,ans=0,c=0;
// 	int ar[3]={0,0,0};
// 	int b[3];
	
// 	if(rank==0)
// 	{
// 		printf("Enter 3 x 3 matrix elements: \n");
// 		for(i=0;i<3;i++)
// 			for(j=0;j<3;j++)
// 				scanf("%d",&a[i][j]);
// 		printf("Enter element to be searched: \n");
// 		scanf("%d",&key);	
// 	}
// 	MPI_Bcast(&key,1,MPI_INT,0,MPI_COMM_WORLD);
	
// 	MPI_Scatter(a,3,MPI_INT,b,3,MPI_INT,0,MPI_COMM_WORLD);
	
// 	for(i=0;i<3;i++)
// 		if(b[i]==key)
// 		{
// 			printf("\n%d found in Process %d",key,rank);
// 			c++;
// 		}
		
// 	//MPI_Gather(&ans,1,MPI_INT,ar,1,MPI_INT,0,MPI_COMM_WORLD);
// 	MPI_Reduce(&c,&ans,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	
// 	if(rank==0)
// 	{
// 	sleep(1);
// 		/*for(i=0;i<3;i++)
// 			printf("%d ",ar[i]);
// 		for(i=0;i<3;i++)
// 			c+=ar[i];*/
// 		printf("\nTotal occurrences of %d is %d\n",key,ans);
// 	}
	
// 	MPI_Finalize();
// 	return 0;
	
// }

#include<stdio.h>
#include<mpi.h>

int main(int argc, char * argv[])
{
	MPI_Init(&argc,&argv);
	int rank,size,k;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	int a[3][3],b[3],i,j,total=0;
	if(rank==0)
	{
		printf("Enter 9 elemnts: \n");
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				scanf("%d",&a[i][j]);
		printf("Enter eleent to be searched: \n");
		scanf("%d",&k);
	}

	MPI_Bcast(&k,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(a,3,MPI_INT,b,3,MPI_INT,0,MPI_COMM_WORLD);
	int c=0;
	for(i=0;i<3;i++)
		if(b[i]==k)
			c++;
	//MPI_Scan(&c,&total,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	MPI_Reduce(&c,&total,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	//printf("Process %d found %d %d times\n",rank,k,total);

	// if(rank==size-1)
	// {
	// 	printf("Total occurences of %d are %d",k,total);
	// }
	if(rank==0)
	{
		printf("Total occurences of %d are %d",k,total);
	}
	MPI_Finalize();
	return 0;
}