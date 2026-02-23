#include<mpi.h>
#include<stdio.h>

int main(int argc, char * argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Status st;
    int size, rank,i,n;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank==0)
    {
        printf("Enter number: \n");
        scanf("%d",&n);
        MPI_Send(&n,1,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Recv(&n,1,MPI_INT,size-1,0,MPI_COMM_WORLD,&st);
        printf("\nReceived %d from last proc\n",n);
    }
    else
    {
        
            MPI_Recv(&n,1,MPI_INT,rank-1,0,MPI_COMM_WORLD,&st);
            printf("Process with rank %d received %d\n",rank,n);
            n++;
            if(rank==size-1)
                MPI_Send(&n,1,MPI_INT,0,0,MPI_COMM_WORLD);
            else
                MPI_Send(&n,1,MPI_INT,rank+1,0,MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}