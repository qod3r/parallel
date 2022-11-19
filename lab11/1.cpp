#include <iostream>
#include "mpi.h"
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    int rank;
    int size;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(0) * rank);
    int n = rand() % 101 - 1;
    int max;
    printf("rank: %d, n: %d\n", rank, n);

    MPI_Reduce(&n, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        usleep(50000);
        printf("rank: %d, max: %d\n", rank, max);
    }

    MPI_Finalize();
    return 0;
}