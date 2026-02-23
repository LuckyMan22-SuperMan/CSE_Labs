#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc,&argv);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int A[5][5];
    int localA[5];
    int fullA[5][5];
    int Brow[5];
    int B[5][5];

    int i,j;

    if(rank==0)
    {
        printf("Enter 5x5 matrix:\n");
        for(i=0;i<5;i++)
            for(j=0;j<5;j++)
                scanf("%d",&A[i][j]);
    }

    /* Scatter rows of A */
    MPI_Scatter(A,5,MPI_INT, localA,5,MPI_INT, 0,MPI_COMM_WORLD);

    /* Collect all rows everywhere */
    MPI_Allgather(localA,5,MPI_INT, fullA,5,MPI_INT, MPI_COMM_WORLD);

    /* compute min & max of column = rank */
    int min = fullA[0][rank];
    int max = fullA[0][rank];

    for(i=1;i<5;i++)
    {
        if(fullA[i][rank] < min)
            min = fullA[i][rank];
        if(fullA[i][rank] > max)
            max = fullA[i][rank];
    }

    /* build row of B */
    for(j=0;j<5;j++)
    {
        if(rank == j)
            Brow[j] = 0;
        else if(rank > j)
            Brow[j] = max;
        else
            Brow[j] = min;
    }

    /* gather rows to root */
    MPI_Gather(Brow,5,MPI_INT, B,5,MPI_INT, 0,MPI_COMM_WORLD);

    if(rank==0)
    {
        printf("\nMatrix B:\n");
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)
                printf("%d ",B[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
