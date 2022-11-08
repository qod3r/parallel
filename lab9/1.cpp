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

    int* a = new int[3];
    for (int i = 0; i < 3; i++) {
        a[i] = rank;
    }

    int* b = new int[3 * (size - 1)];
    int* rc = new int[size];
    int* ds = new int[size];
    for (int i = 0; i < size; i++) {
        rc[i] = 3;
        ds[i] = 3 * (i - 1);
    }
    rc[0] = 0; ds[0] = 0;

    MPI_Gatherv(a, 3, MPI_INT, b, rc, ds, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("rank: %d, b: ", rank);
        for (int i = 0; i < 3 * (size - 1); i++) {
            cout << b[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
