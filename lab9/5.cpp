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
    for (int i = 0; i < size-rank; i++)
        a[i] = rank;

    int** b = new int*[size];
    b[0] = new int[size*size];
    for (int i = 1; i < size; i++)
        b[i] = b[i-1] + size;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            b[i][j] = 0;

    int* rc = new int[size];
    int* ds = new int[size];
    rc[0] = size, ds[0] = 0;
    for (int i = 1; i < size; i++) {
        rc[i] = size - i;
        ds[i] = i * size;
    }

    MPI_Gatherv(a, size-rank, MPI_INT, b[0], rc, ds, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("rank: %d, b:\n", rank);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++)
                cout << b[i][j] << " ";
            cout << endl;
        }
    }

    MPI_Finalize();
    return 0;
}