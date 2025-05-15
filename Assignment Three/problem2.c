#include <stdio.h>
#include<stdlib.h>
#include<math.h>

int main (int argc , char** argv)
{

printf("Enter Size: \n");
int n ;
scanf("%d",&n);

printf("Enter Vector v: \n");
int vec [n];
for (int i = 0; i < n; i++)
{
    int x;
    scanf("%d",&x);
    vec[i]=x;
}

// MEAN
double sum1 =0;
double MEAN=0.0;
for (int i = 0; i < n; i++)
{
    sum1+= vec[i];
}
MEAN= sum1/(double)n;


// Variance
double sum2=0;
double VARAIANCE = 0.0;
for (int i = 0; i < n; i++)
{
    sum2+= pow((vec[i]-MEAN),2);
}
VARAIANCE=sum2/(double)n;

double STANDARDDEVIATION= sqrt(VARAIANCE);

printf("Standard Deviation: %.6f \n",STANDARDDEVIATION);

    return 0;
}