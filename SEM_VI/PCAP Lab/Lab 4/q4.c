#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char * argv[])
{
	MPI_Init(&argc,&argv);
	int rank,size,i,n;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status st;
	
	char str[100],c,ans[100],buf[100];
	char aa[100];
	
	if(rank==0)
	{
		printf("Enter string: \n");
		scanf("%s",str);
		while(strlen(str)!=size)
		{
			printf("Invalid string lengbht enter a string of size %d: \n",size);
			scanf("%s",str);
		}
	}
	
	MPI_Scatter(str,1,MPI_CHAR,&c,1,MPI_CHAR,0,MPI_COMM_WORLD);
	
	for(i=0;i<rank+1;i++)
		buf[i]=c;
	buf[i]='\0';
	//if(rank!=0)
		MPI_Send(buf,rank+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
	
	if(rank==0)
	{
		ans[0]=str[0];
		
		int l=1;
		for(i=1;i<size;i++)
		{
			MPI_Recv(&ans[l],i+1,MPI_CHAR,i,0,MPI_COMM_WORLD,&st);
			l+=i+1;
		}
		ans[l]='\0';
		
		printf("\n%s is the received string\n",ans);
		
	}
	
	MPI_Finalize();
	return 0;
	
}
