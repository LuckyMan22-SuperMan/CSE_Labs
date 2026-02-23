#include<stdio.h>
#include<mpi.h>

int main(int argc, char * argv[])
{
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Status st;
    int buf_size=100;
    int i,n,a[size-1],buf[buf_size],res;

    MPI_Buffer_attach(buf,buf_size);
    if(rank==0)
    {
        printf("Enter %d elements: \n",size-1);
        for(i=0;i<size-1;i++)
            scanf("%d",&a[i]);
        int k=0;
        for(i=1;i<size;i++)
            MPI_Bsend(&a[k++],1,MPI_INT,i,i,MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&n,1,MPI_INT,0,rank,MPI_COMM_WORLD,&st);
        if(rank%2==0)
            res=n*n;
        else
            res=n*n*n;
        printf("Process %d received %d and result is %d\n",rank,n,res);
        
    }
    MPI_Buffer_detach(buf,&buf_size);
    MPI_Finalize();
    return 0;
}