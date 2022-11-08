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

    int* a = new int[size-rank];
    for (int i = 0; i < size-rank; i++) {
        a[i] = rank;
    }

    int bsize = size * (size + 1)/2;
    int* b = new int[bsize];
    int* rc = new int[size];
    int* ds = new int[size];

    rc[0] = size, ds[0] = 0;
    for (int i = 1; i < size; i++) {
        rc[i] = size - i;
        ds[i] = rc[i-1] + ds[i-1];
    }

    MPI_Gatherv(a, size-rank, MPI_INT, b, rc, ds, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("rank: %d, b: ", rank);
        for (int i = 0; i < bsize; i++) {
            cout << b[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}