// #include<mpi.h>
// #include<stdio.h>

// int main(int argc, char * argv[])
// {
//     int size,rank,i,n;
//     MPI_Init(&argc,&argv);
//     MPI_Status st;
//     MPI_Comm_rank(MPI_COMM_WORLD,&rank);
//     MPI_Comm_size(MPI_COMM_WORLD,&size);

//     if(rank==0)
//     {
//         printf("Enter a no to send: \n");
//         scanf("%d", &n);
//         for(i=1;i<size;i++)
//             MPI_Send(&n,1,MPI_INT,i,0,MPI_COMM_WORLD);
//     }
//     else
//     {
//         MPI_Recv(&n,1,MPI_INT,0,0,MPI_COMM_WORLD,&st);
//         printf("\nProcess %d received %d\n",rank,n);
//     }

//     MPI_Finalize();
//     return 0;
// }

#include<mpi.h>
#include<stdio.h>

int main(int argc, char * argv[])
{
    int size,rank,i,n;
    MPI_Init(&argc,&argv);
    MPI_Status st;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    //int n;
    if(rank==0)
    {
        printf("Enter number to send: \n");
        scanf("%d",&n);
        for(i=1;i<size;i++)
            MPI_Ssend(&n,1,MPI_INT,i,i,MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&n,1,MPI_INT,0,rank,MPI_COMM_WORLD,&st);
        printf("Proc %d received %d\n",rank, n);
    }

    MPI_Finalize();
    return 0;


}