#include<stdio.h>
#include<mpi.h>

int main(int argc, char * argv[])
{
    MPI_Init(&argc,&argv);
    int size,rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int a[3][3],b[3],c[3][3],i,j,n;
    if(rank==0)
    {
        printf("Enter 9 elements: \n");
        for(i=0;i<3;i++)
            for(j=0;j<3;j++)
                scanf("%d",&a[i][j]);
    }
   MPI_Scatter(a,3,MPI_INT,b,3,MPI_INT,0,MPI_COMM_WORLD);
    //MPI_Reduce(b,c,3,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    // for(i=0;i<3;i++)
    //     MPI_Scatter(&a[i][0],1,MPI_INT,&b[i],1,MPI_INT,0,MPI_COMM_WORLD);
    
    // for(i=0;i<3;i++)
    //     n+=b[i];
    // MPI_Gather(&n,1,MPI_INT,c,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Gather(b,3,MPI_INT,c,3,MPI_INT,0,MPI_COMM_WORLD);

    if(rank==0)
    {
        printf("array received is: \n");
        for(i=0;i<3;i++)
            {for(j=0;j<3;j++)
                printf("%d ",c[i][j]);
            printf("\n");
            }
    }

    MPI_Finalize();
    return 0;
}