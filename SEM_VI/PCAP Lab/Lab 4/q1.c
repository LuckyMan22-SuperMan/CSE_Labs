#include<stdio.h>
#include<mpi.h>
#include<unistd.h>
#include<stdlib.h>
void ErrorHandler(int code)
{
	if(code!=MPI_SUCCESS)
	{
		char string[100];
		int n;
		
		
		MPI_Error_string(code,string,&n);
		printf("Error is hahaah: %s",string);
		
	}
	return;
}

int main(int argc, char * argv[])
{
	int err,size,rank,n,Mc;
	MPI_Init(&argc,&argv);
	//MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
	
	err=MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	


	int buf_size=100;
    int i,n,a[size-1],buf[buf_size],res,rec,a[3],b[3];
	MPI_Status st;
    MPI_Buffer_attach(buf,buf_size);
	MPI_Send(&n,1,MPI_INT,2,0,MPI_COMM_WORLD);
	MPI_Buffer_detach(buf,&buf_size);
	MPI_Recv(&rec,1,MPI_INT,0,0,MPI_COMM_WORLD,&st);

	MPI_Scatter(a,3,MPI_INT,b,3,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Gather(a,3,MPI_INT,b,3,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
	
	MPI_Reduce(a,b,3,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Scan(a,b,3,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	
	int i,ans,fact,f;
	
	f=rank+1;
	MPI_Scan(&f,&fact,1,MPI_INT,MPI_PROD,MPI_COMM_WORLD);
	MPI_Scan(&fact,&ans,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	ErrorHandler(err);
	
	if(rank==size-1)
		printf("\nResult is %d\n",ans);
		
	MPI_Finalize();
	return 0;
}

// #include<stdio.h>
// #include<mpi.h>
// void ErrorHandler(int code)
// {
// 	if(code!=MPI_SUCCESS)
// 	{
// 		char str[100];
// 		int n;

// 		MPI_Error_string(code,str,&n);
// 		printf("Error is haha: %s",str);
// 	}
// }
// int main(int argc, char * argv[])
// {
// 	MPI_Init(&argc,&argv);
// 	int size,rank,i,err;
// 	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
// 	MPI_Comm_size(MPI_COMM_WORLD,&size);

// 	MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);

// 	int fsum=0,fact=1;
// 	int f=rank+1;
// 	// for(i=1;i<=rank+1;i++)
// 	// 	fact*=i;
// 	err= MPI_Scan(&f,&fact,1,MPI_INT,MPI_PROD,MPI_COMM_WORLD);	//
// 	ErrorHandler(err);
// 	MPI_Scan(&fact,&fsum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
// 	if(rank==size-1)
// 	{
// 		printf("Ans is %d",fsum);
// 	}

// 	MPI_Finalize();
// 	return 0;
// }




