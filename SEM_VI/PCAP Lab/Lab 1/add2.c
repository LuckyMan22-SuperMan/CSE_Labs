#include<stdio.h>
#include<mpi.h>
#include<math.h>

int isprime(int n)
{
    int i;
    for(i=2;i<=n/2;i++)
        if(n%i==0)
            return 0;
    return 1;
}

int main(int argc, char * argv[])
{
    MPI_Init(&argc,&argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(rank==0)
    {
        printf("Starting..");
    }
    else
    {
        for(int i=2;i<=100;i++)
            if(isprime(i))
                printf("%d, ",i);
    }

    MPI_Finalize();
    return 0;
}