#include "mpi/mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int n = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("rank: %d, size = %d\n", rank, size);

    MPI_Finalize();
    return 0;
}

