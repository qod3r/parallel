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

    int n;
    if (rank == 0)
        cin >> n;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int* a = new int[2 * n * size];
    for (int i = 0; i < 2*n*size; i++) {
        a[i] = i;
    }
    int* b = new int[2 * n];
    int* rc = new int[size];
    for (int i = 0; i < size; i++)
        rc[i] = 2 * n;

    MPI_Reduce_scatter(a, b, rc, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    usleep(rank * 100000);
    printf("rank: %d, b: ", rank);
    for (int i = 0; i < 2*n; i++) {
        cout << b[i] << " ";
    }
    cout << endl;

    delete [] a, b, rc;
    MPI_Finalize();
    return 0;
}