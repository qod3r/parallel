#include <iostream>
#include "mpi.h"
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    int rank;
    int size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int** a = new int*[size];
    a[0] = new int[size * size];
    for (int i = 1; i < size; i++)
        a[i] = a[i-1] + size;
    if (rank == 0)
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                a[i][j] = i;

    int* b = new int[size - rank];

    int* sc = new int[size];
    int* ds = new int[size];
    for (int i = 0; i < size; i++) {
        sc[i] = size - i;
        ds[i] = i * size;
    }

    MPI_Scatterv(a[0], sc, ds, MPI_INT, b, size - rank, MPI_INT, 0, MPI_COMM_WORLD);

    usleep(rank * 100000);
    printf("rank: %d, b: ", rank);
    for (int i = 0; i < size - rank; i++) {
        cout << b[i] << " ";
    }
    cout << endl;

    delete [] a[0], a, b, sc, ds;
    MPI_Finalize();
    return 0;
}
