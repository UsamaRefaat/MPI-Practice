#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

//USE SEND AND RECEIVE ONLY

int main (int argc , char** argv)
{
    

    MPI_Init( NULL , NULL);
    int rank, psize;
    MPI_Comm_rank( MPI_COMM_WORLD , &rank);
    MPI_Comm_size( MPI_COMM_WORLD , & psize);
    MPI_Status status;
    
   if (rank==0)
   {
        int encode = -1;
        printf("choose \n 1. encode \n 2. decode \n");
        int choice;
        scanf("%d",&choice);
        getchar();
        if (choice==1)
        {
            encode=1;
        }
        else if (choice == 2)
        {
            encode =0;
        }
        else
        {
            printf(" wrong choice \n program terminating... ");
            return 0 ;
        }

        printf(" input from \n 1. console \n 2. file \n");
        int inputchoice;
        scanf("%d",&inputchoice);
        getchar(); // remove new line char left from scanf
        char *str = NULL;
        size_t size = 0;

        if(inputchoice==1)
        {

            size_t len = 0;
            printf("Enter a string: ");
            getline(&str, &len, stdin);
            size = strlen(str);
            if(str[size-1]=='\n') 
            {
                str[size-1] = '\0';
            }
            size = strlen(str); // after the if condition

        }

        else if (inputchoice==2)
        {
            char filename [50];
            printf("enter file name, make sure it exists\n");
            scanf("%s", filename);
            getchar();
            FILE *pointertofile = fopen(filename , "r");
            if (!pointertofile)
            {
                printf(" FILE DOES NOT EXIST \n Program terminating... ");
                return 0;
            }

            fseek(pointertofile,0,SEEK_END);
            size = ftell(pointertofile);
            rewind(pointertofile);

            str = malloc(size+1);
            fread(str , 1 , size , pointertofile);
            str[size]= '\0';
            fclose(pointertofile);
            
        }

        else
        {
            printf(" wrong choice \n program terminating... ");
            return 0 ;   
        }
        

        printf(" output in \n 1. console \n 2. file\n");
        int console =-1;
        int output;
        scanf("%d",&output);
        getchar();

        if (output==1)
        {
            console=1;
        }
        else if (output==2)
        {
            console=0;
        }
        else
        {
            printf(" wrong choice \n program terminating... ");
            return 0 ;   
        }

        if(size == 0)
        {
            printf("Empty String!! \n Program terminating...");
            MPI_Finalize();
            return 0;
        }

        int elementsperprocess =  size / (psize-1);
        int remainder = size%(psize-1);
        int offset =0;
        
        for (int i = 1; i < psize; i++)
        {
            
            int sentcounter = elementsperprocess + (i<=remainder ? 1:0);
            MPI_Send( &sentcounter , 1 , MPI_INT , i , 45 , MPI_COMM_WORLD);
            MPI_Send( str+offset , sentcounter , MPI_CHAR , i , 22 , MPI_COMM_WORLD);
            MPI_Send( &offset , 1 , MPI_INT , i , 74 , MPI_COMM_WORLD);
            MPI_Send( &encode , 1 , MPI_INT , i , 14 , MPI_COMM_WORLD);
            offset+=sentcounter;
            
        }

        
        for (int i = 1; i < psize; i++)
        {
            char *reciver;
            int currsize , startindex;
            MPI_Recv( &currsize , 1 , MPI_INT , i , 100 , MPI_COMM_WORLD , & status);
            MPI_Recv( &startindex , 1 , MPI_INT , i , 12 , MPI_COMM_WORLD , & status);

            reciver = malloc(currsize * sizeof(char));
            MPI_Recv( reciver , currsize , MPI_CHAR , i , 101 , MPI_COMM_WORLD , & status);
            for (int j = 0; j < currsize; j++)
            {
                str[startindex + j] = reciver[j];
            }
            
            free(reciver);
        }
        if (console)
        {
            printf ("%s\n",str);
            
        }
        else
        {
            FILE *pointertofileout = fopen("output.txt","w");
            fprintf(pointertofileout , "%s\n" ,str );
            fclose(pointertofileout);
            printf("output is written to a file named output.txt ... \n");    
        }        
        free(str);

   }

   else
   {

        int localsize ;
        MPI_Recv(  &localsize, 1 , MPI_INT , 0 , 45 , MPI_COMM_WORLD , &status);

        int offset;
        MPI_Recv(  &offset, 1 , MPI_INT , 0 , 74 , MPI_COMM_WORLD , &status);

        char *localstr;
        localstr = malloc(localsize*sizeof(char));
        MPI_Recv( localstr , localsize , MPI_CHAR , 0 , 22 , MPI_COMM_WORLD ,  &status);
        localstr[localsize] = '\0';

        int encode;
        MPI_Recv(  &encode, 1 , MPI_INT , 0 , 14 , MPI_COMM_WORLD , &status);

            for (int i = 0; i < localsize; i++)
            {
                // ignore anything rather than the alphabet
                if (encode)
                {
                    if(localstr[i]>='a' && localstr[i]<='z')
                    localstr[i] = 'a' + (localstr[i] - 'a' +3) % 26;
                    else if (localstr[i]>='A' && localstr[i]<='Z')
                    localstr[i] = 'A' + (localstr[i] - 'A' +3) % 26;
                }
                else
                {
                    if(localstr[i]>='a' && localstr[i]<='z')
                    localstr[i] = 'a' + (localstr[i] - 'a' -3+26) % 26;
                    else if (localstr[i]>='A' && localstr[i]<='Z')
                    localstr[i] = 'A' + (localstr[i]- 'A' - 3+26) % 26;
                }

            }

            MPI_Send( &localsize , 1, MPI_INT , 0 , 100 , MPI_COMM_WORLD);
            MPI_Send( localstr , localsize , MPI_CHAR , 0 , 101 , MPI_COMM_WORLD);
            MPI_Send( &offset , 1 , MPI_INT , 0 , 12 , MPI_COMM_WORLD);
            free(localstr);
   }
   
    MPI_Finalize();
    return 0;
}
