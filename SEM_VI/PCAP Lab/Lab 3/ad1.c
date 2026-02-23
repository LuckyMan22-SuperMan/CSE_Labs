#include<mpi.h>
#include<stdio.h>
#include<math.h>
int main(int argc, char * argv[])
{
	int size,rank,ch;
	MPI_Init(&argc,&argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
    int a[100],b[100],ans[100];
    int m,i,n;

    if(rank==0)
    {
        printf("Enter value of m: \n");
        scanf("%d",&m);
        printf("Enter %d x %d= %d elements\n",m,size,m*size);
        for(i=0;i<m*size;i++)
            scanf("%d",&a[i]);
    }
    MPI_Bcast(&m,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Scatter(a,m,MPI_INT,b,m,MPI_INT,0,MPI_COMM_WORLD);
    for(i=0;i<m;i++)
    {
        b[i]=pow(b[i],rank+2);
    }
    MPI_Gather(b,m,MPI_INT,ans,m,MPI_INT,0,MPI_COMM_WORLD);

    if(rank==0)
    {
        printf("Received: \n");
        for(i=0;i<size*m;i++)
            printf("%d, ",ans[i]);
        
    }
    MPI_Finalize();
    return 0;
}