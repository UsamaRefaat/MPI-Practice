#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include <omp.h>
#include<mpi.h>
#include <time.h>

int main (int argc , char** argv){

MPI_Init( NULL , NULL);

int worldsize;
MPI_Comm_size( MPI_COMM_WORLD, &worldsize);

int worldrank;
MPI_Comm_rank( MPI_COMM_WORLD , &worldrank);

char processor_name [MPI_MAX_PROCESSOR_NAME];
int namelen ;
MPI_Get_processor_name(processor_name , &namelen);

long long psum =0 ;
long long totalsum=0;
int elementperprocess;
int remainder;
int startindex;
int *array;
int *partialarray;
int n = 10000003;
MPI_Status status;

if(worldrank ==0)
{
        array = malloc(n*sizeof(int));
        for (int i = 0; i <n; i++)
        {
            array[i]=i+1;
        }
        
        elementperprocess = n/worldsize;
        remainder = n%worldsize;
        int offset=0;
        int mastercount = elementperprocess + (remainder>0?1:0);

        #pragma omp parallel for reduction(+:psum)
        for (int i = 0; i < mastercount; i++)
        {
            psum+=array[i];
        }

        offset+=mastercount;
        if (remainder > 0) {
            remainder--;
        }

        for (int i = 1; i < worldsize; i++)
        {
            int sentcount = elementperprocess + (i<remainder+1? 1:0);
            MPI_Send(&sentcount, 1, MPI_INT , i , 12 , MPI_COMM_WORLD);
            MPI_Send(array+offset, sentcount, MPI_INT , i , 13 , MPI_COMM_WORLD);  
            offset+=sentcount;  
        }
        
        MPI_Reduce( &psum, &totalsum , 1 , MPI_LONG_LONG , MPI_SUM , 0 , MPI_COMM_WORLD);
        printf("total sum of the array: %lld \n",totalsum);
        free(array);
        
}
 
else
{
        int recivedcount;
        MPI_Recv( &recivedcount , 1 , MPI_INT , 0 , 12 , MPI_COMM_WORLD  ,  &status);
        partialarray = malloc(recivedcount*sizeof(int));
        MPI_Recv( partialarray , recivedcount , MPI_INT , 0 , 13 , MPI_COMM_WORLD  ,  &status);

        #pragma omp parallel for reduction(+:psum)
        for (int i = 0; i < recivedcount ; i++)
        {
            psum+=partialarray[i];
        } 
        MPI_Reduce( &psum, &totalsum , 1 , MPI_LONG_LONG , MPI_SUM , 0 , MPI_COMM_WORLD);

}

MPI_Finalize();
return 0;
}