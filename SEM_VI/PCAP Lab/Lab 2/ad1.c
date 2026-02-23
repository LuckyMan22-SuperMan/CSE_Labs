#include<stdio.h>
#include<mpi.h>

int isprime(int n)
{
    int i;
    if(n==1)
        return 0;
    for(i=2;i<=n/2;i++)
        if(n%i==0)
            return 0;
    return 1;
}

int main(int argc, char * argv[])
{
    MPI_Init(&argc,&argv);
    int rank,i,size,x;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Status st;

    int a[100],ans[100];
    int n;
    if(rank==0)
    {
        printf("Enter %d elements: \n",size);
        for(i=0;i<size;i++)
            scanf("%d",&a[i]);
        for(i=1;i<size;i++)
            MPI_Send(&a[i],1,MPI_INT,i,0,MPI_COMM_WORLD);
        x=isprime(a[0]);
        printf("Process %d received %d and it is prime=%d\n",rank,a[0],x);
    }
    else
    {
        MPI_Recv(&n,1,MPI_INT,0,0,MPI_COMM_WORLD,&st);
        x=isprime(n);
        printf("Process %d received %d and it is prime=%d\n",rank,n,x);
    }
    MPI_Finalize();
    return 0;
}