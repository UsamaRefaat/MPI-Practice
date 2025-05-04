#include<stdio.h>
#include <stdlib.h>
#include <mpi.h>

//USE SEND AND RECEIVE ONLY

int main (int argc , char** argv)
{
    
    MPI_Init( NULL , NULL);
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD , &rank);
    MPI_Comm_size( MPI_COMM_WORLD , & size);
    MPI_Status status;

    if(rank==0)
    {

            printf("Hello from master process.\n");
            printf("Number of slave processes is %d\n\n",size-1);
            int n ;
            printf("Please enter size of array...\n");
            scanf("%d",&n);
            if(n<= 0 )
            {
                printf("Array size must be greater than zero, program terminating....");
                return 0;
            }
            int *arr = malloc(n*sizeof(int));
            printf("\nPlease enter array elements ...\n");
            for (int i = 0; i < n; i++)
            {
                scanf("%d", &arr[i]);
            }
            printf("\n");
            int elementsperprocess = n / (size-1);
            int remainder = n%(size-1);
            int offset =0;

            for (int i = 1; i < size; i++)
            {  
                int sentcounter = elementsperprocess + (i<= remainder ? 1:0);
                MPI_Send( &sentcounter , 1 , MPI_INT , i , 45 , MPI_COMM_WORLD);

                if (sentcounter>0)
                {
                MPI_Send( arr+offset , sentcounter , MPI_INT , i , 22 , MPI_COMM_WORLD);
                MPI_Send( &offset , 1 , MPI_INT , i , 7 , MPI_COMM_WORLD);
                offset+=sentcounter;
                } 
            }
            int result[2]; 
            int globalmax=-1e9;
            int globalmaxindex=-1;

            for (int i = 1; i < size; i++)
            {
                int sentcounter = elementsperprocess + (i<= remainder ? 1:0);
                if (sentcounter == 0) // to skip recieveing from the idle process when the size of the array is less than size of processes , program will not terminate
                {
                        continue;
                }
                
                MPI_Recv( result , 2 , MPI_INT , i , 74 , MPI_COMM_WORLD , &status);

                if (result[0]>globalmax)
                {
                    globalmax = result[0];
                    globalmaxindex=result[1];

                }
            }
            printf("Master process announce the final max which is %d and its index is %d.\n\n",globalmax,globalmaxindex);
            printf("Thanks for using our program\n");
            free(arr);
    }

    else
    {
            int localsize ;
            MPI_Recv(  &localsize, 1 , MPI_INT , 0 , 45 , MPI_COMM_WORLD , &status);
            if (localsize==0) // because when the size of the array is less than size of processes , program will not terminate
            {
                printf("Slave#%d is idle.\n",rank);
                MPI_Finalize();
                return 0;
            }
            
            int *localarr;
            localarr = malloc(localsize*sizeof(int));
            MPI_Recv( localarr , localsize , MPI_INT , 0 , 22 , MPI_COMM_WORLD ,  &status);
            int offset ;
            MPI_Recv( &offset , 1 , MPI_INT , 0 , 7 , MPI_COMM_WORLD , &status);
            int maxi= -1e9;
            int localindex = -1;
            for (int i = 0; i < localsize; i++)
            {
                if (localarr[i]>maxi)
                {
                    maxi= localarr[i];
                    localindex=i;
                }

            }
            int globalindex = localindex+offset;
            printf("Hello from slave#%d Max number in my partition is %d and index is %d.\n",rank,maxi,localindex);
            int localresult[2] = {maxi,globalindex};
            MPI_Send( localresult , 2 , MPI_INT , 0 , 74 , MPI_COMM_WORLD);
            free(localarr);
    }

    MPI_Finalize();
    return 0;
}
