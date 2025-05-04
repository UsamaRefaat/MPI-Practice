// subrange * (rank-1) + x ===> Formula to calculate the Starting index in slave
// p 1 ==> 5 * 0 + 1 = 1
// p 2 ==> 5 * 1 + 1 = 6
// p 3 ==> 5 * 2 + 1 = 11
// end index = starting + subrange

// USING SEND AND RECV ONLY

#include <stdio.h>
#include <mpi.h>
#include<stdlib.h>
#include<math.h>


int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}


int main (int argc , char** argv)
{

    MPI_Init( NULL , NULL);
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD , &rank);
    MPI_Comm_size( MPI_COMM_WORLD , & size);
    MPI_Status status;
    double start , end , elapsed;
    int x , y ;
    if(rank ==0)
    {
        start= MPI_Wtime();
        int subrange, remainder, totalcount=0;
        printf("Enter Lower Bound: \n");
        scanf("%d", &x);
        printf("Enter Upper Bound: \n");
        scanf("%d", &y);
        subrange = (y - x) / (size - 1);
        remainder = (y-x) % (size-1);
        for (int i = 1; i < size; i++)
        {
            MPI_Send( &subrange , 1 , MPI_INT , i , 22 , MPI_COMM_WORLD);
            MPI_Send( &x , 1 , MPI_INT , i , 44 , MPI_COMM_WORLD);
            MPI_Send( &remainder , 1 , MPI_INT , i , 66 , MPI_COMM_WORLD);

        }
        

        for (int i = 1; i < size; i++)
        {
            int localcount;
            MPI_Recv(&localcount, 1 , MPI_INT , i , 88 , MPI_COMM_WORLD, & status);
            totalcount+=localcount;
        }
        end= MPI_Wtime();
        elapsed = end - start;
        printf("Total Time: %f seconds \n", elapsed);
        printf("Total number of primes between %d and %d is: %d \n",x,y,totalcount);

    }

    else
    {
        int subrange, x, remainder ,partialcount=0;

        MPI_Recv( &subrange, 1 , MPI_INT , 0 , 22 , MPI_COMM_WORLD ,  &status);
        MPI_Recv( &x, 1 , MPI_INT , 0 , 44 , MPI_COMM_WORLD,  &status);
        MPI_Recv( &remainder, 1 , MPI_INT , 0 , 66 , MPI_COMM_WORLD,  &status);

        int startindex = subrange * (rank - 1) + x ;
        int endindex = startindex + (subrange-1);
        if( (rank-1) < remainder)
        {
            startindex += rank-1;
            endindex += rank;
        }
        else
        {
            startindex+=remainder;
            endindex+=remainder;
        }
        printf("process %d from %d to %d \n",rank,startindex,endindex);
        if (endindex > y) endindex = y;
        for (int i = startindex ; i<= endindex; i++)
        {
            if(i==endindex && is_prime(endindex+1)) partialcount++; 
            if(is_prime(i)) partialcount++;
        }

       MPI_Send( &partialcount , 1 , MPI_INT , 0 , 88 , MPI_COMM_WORLD);


    }



    return 0 ;
}
