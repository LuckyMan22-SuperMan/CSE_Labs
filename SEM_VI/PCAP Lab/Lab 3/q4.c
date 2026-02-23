#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char * argv[])
{
	int size,rank,i,ch;
	MPI_Init(&argc,&argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	char str1[100],str2[100],s1[100],s2[100],ss[100],ans[100];

	if(rank==0)
	{
		printf("Enter the two strings: \n");
		scanf("%s%s",str1,str2);
		while(strlen(str1)!=strlen(str2) || strlen(str1)%size!=0)
		{
			printf("Enter the two strings again: \n");
		scanf("%s%s",str1,str2);
		}

		ch=strlen(str1)/size;
	}

	MPI_Bcast(&ch,1,MPI_INT,0,MPI_COMM_WORLD);

	MPI_Scatter(str1,ch,MPI_CHAR,s1,ch,MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Scatter(str2,ch,MPI_CHAR,s2,ch,MPI_CHAR,0,MPI_COMM_WORLD);

	s1[ch]='\0';
	s2[ch]='\0';
	int k=0;
	i=0;
	while(s1[i]!='\0' && s2[i]!='\0')
	{
		ss[k++]=s1[i];
		ss[k++]=s2[i];
		i++;
	}
	ss[k]='\0';

	MPI_Gather(ss,2*ch,MPI_CHAR,ans,k,MPI_CHAR,0,MPI_COMM_WORLD);

	if(rank==0)
	{
		ans[size*2*ch]='\0';
		printf("Process %d reeceived %s",rank,ans);
	}

	MPI_Finalize();
	return 0;
}