#include <stdio.h>
#include <stdlib.h>
#include <openmpi/mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int n, m, np, pid, mult, *arr1, *arr2, *parArr1, *parArr2, *parans, *ans, ElementsPerProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    if (pid == 0)
    {
        scanf("%d %d", &n, &m);
        mult = n * m;
        if ((mult) % np != 0)
        {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        else
        {
            arr1 = (int *)malloc(sizeof(int) * mult);
            arr2 = (int *)malloc(sizeof(int) * mult);
            ans = (int *)malloc(sizeof(int) * mult);
            for (int i = 0; i < mult; i++)
            {
                scanf("%d", &arr1[i]);
            }
            for (int i = 0; i < mult; i++)
            {
                scanf("%d", &arr2[i]);
            }
            ElementsPerProcess = mult / np;
        }
    }
    MPI_Bcast(&ElementsPerProcess, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&mult, 1, MPI_INT, 0, MPI_COMM_WORLD);
    parArr1 = (int *)malloc(sizeof(int) * ElementsPerProcess);
    parArr2 = (int *)malloc(sizeof(int) * ElementsPerProcess);
    parans = (int *)malloc(sizeof(int) * ElementsPerProcess);
    MPI_Scatter(arr1, ElementsPerProcess, MPI_INT, parArr1, ElementsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(arr2, ElementsPerProcess, MPI_INT, parArr2, ElementsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < ElementsPerProcess; i++)
    {
        parans[i] = parArr1[i] + parArr2[i];
    }
    MPI_Gather(parans, ElementsPerProcess, MPI_INT, ans, ElementsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);
    if (pid == 0)
    {
        for (int i = 0; i < mult; i++)
        {
            printf("%d ", ans[i]);
        }
    }
    MPI_Finalize();
}