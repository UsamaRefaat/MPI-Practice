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

printf("Enter Array Elements: \n");
int* vec = malloc(n * sizeof(int));
for (int i = 0; i < n; i++)
{
    int x;
    scanf("%d",&x);
    vec[i]=x;
}

// MEAN
double sum1 =0.0;
double MEAN=0.0;

#pragma omp parallel for reduction(+:sum1)
for (int i = 0; i < n; i++)
{
    sum1+= vec[i];
}
MEAN= sum1/(double)n;


// Variance
double sum2=0.0 ;
double VARAIANCE = 0.0;

#pragma omp parallel for reduction(+:sum2)
for (int i = 0; i < n; i++)
{
    sum2+= pow((vec[i]-MEAN),2);
}
VARAIANCE=sum2/(double)n;
double STANDARDDEVIATION= sqrt(VARAIANCE);

printf("Standard Deviation: %.6f \n",STANDARDDEVIATION);
free(vec);
return 0;
}