#include <stdio.h>
#include<stdlib.h>
#include<math.h>

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
for(int i = 0 ; i<n; i++) 
{
for (int j = 0; j < n; j++)
{
    res[i]+= vec[j] * mat[i][j];
}
}

printf("Resulting vector r: ");
for (int j = 0; j < n; j++)
{
    printf  ( "%d ", res[j] );
}

return 0;
}