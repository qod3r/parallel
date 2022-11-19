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

    int* a = new int[size * (size + 1)];
    if (rank == 0) {
        for (int i = 0; i < size * (size + 1); i++) {
            a[i] = i;
        }
    }
    int* b = new int[rank*2 + 2];
    int* sc = new int[size];
    int* ds = new int[size];
    for (int i = 0; i < size; i++) {
        sc[i] = 2*i + 2;
        ds[i] = i * (i + 1);
    }

    MPI_Scatterv(a, sc, ds, MPI_INT, b, rank*2 + 2, MPI_INT, 0, MPI_COMM_WORLD);

    usleep(rank * 100000);
    printf("rank: %d, b: ", rank);
    for (int i = 0; i < rank*2 + 2; i++) {
        cout << b[i] << " ";
    }
    cout << endl;

    delete [] a, b, sc, ds;
    MPI_Finalize();
    return 0;
}
