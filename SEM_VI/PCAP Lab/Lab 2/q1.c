// #include<mpi.h>
// #include<stdio.h>
// #include<string.h>
// #include<stdlib.h>

// int main(int argc, char * argv[])
// {
//     MPI_Init(&argc,&argv);
//     MPI_Status st;
//     int size,rank,n,i;
//     char str[100],buf[100];

//     MPI_Comm_rank(MPI_COMM_WORLD,&rank);
//     MPI_Comm_size(MPI_COMM_WORLD,&size);

//     if(rank==0)
//     {
//         printf("Enter string: \n");
//         scanf("%s",str);
       
//         n=strlen(str);

//         MPI_Ssend(&n,1,MPI_INT,1,0,MPI_COMM_WORLD);
//         MPI_Ssend(str,n,MPI_CHAR,1,0,MPI_COMM_WORLD);

        
//         MPI_Recv(str,n,MPI_CHAR,1,1,MPI_COMM_WORLD,&st);
//         str[n]='\0';

//         printf("\nReceived modified string: %s",str);
//     }
//     else
//     {
//         MPI_Recv(&n,1,MPI_INT,0,0,MPI_COMM_WORLD,&st);
//         MPI_Recv(str,n,MPI_CHAR,0,0,MPI_COMM_WORLD,&st);
//         str[n]='\0';

//         for(i=0;i<n;i++)
//         {
//             if(str[i]>='a' && str[i]<='z')
//                 str[i]-=32;
//             else if(str[i]>='A' && str[i]<='Z')
//                 str[i]+=32;
//         }
//         printf("\nProcess %d has modified sucessully now sending",rank);
//         fflush(stdout);
//         MPI_Send(str,n,MPI_CHAR,0,1,MPI_COMM_WORLD);
//     }

//     MPI_Finalize();

//     return 0;
    
// }
#include<stdio.h>
#include<mpi.h>
#include<string.h>
int main(int argc, char * argv[])
{
    MPI_Init(&argc,&argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status st;

    char str[100],ans[100];
    int n;
    if(rank==0)
    {
        printf("Enter word: \n");
        scanf("%s",str);
        n=strlen(str);
        MPI_Send(&n,1,MPI_INT,1,1,MPI_COMM_WORLD);
        MPI_Ssend(str,strlen(str),MPI_CHAR,1,0,MPI_COMM_WORLD);
        

        MPI_Recv(ans,strlen(str),MPI_CHAR,1,2,MPI_COMM_WORLD,&st);
        ans[strlen(str)]='\0';
        printf("Received: %s",ans);
    }
    else
    {
        MPI_Recv(&n,1,MPI_INT,0,1,MPI_COMM_WORLD,&st);
        MPI_Recv(str,n,MPI_CHAR,0,0,MPI_COMM_WORLD,&st);
        str[n]='\0';
        for(int i=0;i<n+1;i++)
        {
            if(str[i]>='a' && str[i]<='z')
                str[i]-=32;
            else if(str[i]>='A' && str[i]<='Z')
                str[i]+=32;
        }
        MPI_Ssend(str,n,MPI_CHAR,0,2,MPI_COMM_WORLD);
    }
}