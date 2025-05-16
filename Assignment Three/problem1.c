#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int main (int argc , char** argv)
{
printf("Enter Size: \n");
int n ;
scanf("%d",&n);

int** mat = malloc(n * sizeof(int*));
for (int i = 0; i < n; i++) {
    mat[i] = malloc(n * sizeof(int));
}

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
int* vec = malloc(n * sizeof(int));
int* res = malloc(n * sizeof(int));

for (int i = 0; i < n; i++)
{
    int x;
    scanf("%d",&x);
    vec[i]=x;
}

for (int i = 0; i < n; i++)
{
    res[i]=0;
}

#pragma omp parallel for
for(int i = 0 ; i<n; i++) 
{
for (int j = 0; j < n; j++)
{
    res[i]+= vec[j] * mat[i][j];
}
}

printf("Resulting vector r: \n");
for (int j = 0; j < n; j++)
{
    printf  ( "%d ", res[j] );
}
    printf("\n");

for (int i = 0; i < n; i++) {
    free(mat[i]);
}
    free(mat);
    free(vec);
    free(res);
    return 0;
}