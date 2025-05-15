#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include <omp.h>
#include <time.h>

int main (int argc , char** argv)
{
printf("Enter Size: \n");
int n ;
scanf("%d",&n);

int mat [n][n] ;
printf("Enter Matrix A: \n");
for(int i = 0 ; i<n; i++) 
{
for (int j = 0; j < n; j++)
{
    int x; 
    scanf("%d",&x);
    mat[i][j]=x;
}
}

printf("Enter Vector v: \n");
int vec [n];
for (int i = 0; i < n; i++)
{
    int x;
    scanf("%d",&x);
    vec[i]=x;
}

int res [n];
for (int i = 0; i < n; i++)
{
    res[i]=0;
}
clock_t start, end;
start = clock();
#pragma omp parallel for
for(int i = 0 ; i<n; i++) 
{
for (int j = 0; j < n; j++)
{
    res[i]+= vec[j] * mat[i][j];
}
}
end= clock();
double total = (double) (end-start)/ CLOCKS_PER_SEC;
printf("total time %f\n",total);
printf("Resulting vector r: ");
for (int j = 0; j < n; j++)
{
    printf  ( "%d ", res[j] );
}
printf("\n");

return 0;
}