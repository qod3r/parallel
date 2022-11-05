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

    int **a = new int*[size];
    a[0] = new int[size*size];
    for (int i = 1; i < size; i++)
        a[i] = a[i-1] + size;

    int* b = new int[rank+1];
    for (int i = 0; i < rank+1; i++) {
        b[i] = rank;
    }

    int* rc = new int[size];
    int* ds = new int[size];
    for (int i = 0; i < size; i++) {
        rc[i] = i + 1;
        ds[i] = i * size;
    }
    MPI_Gatherv(b, rank+1, MPI_INT, a[0], rc, ds, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Gather(b, n, MPI_INT, a[0], n, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Allgather(&rank, 1, MPI_INT, a, 1, MPI_INT, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("rank: %d, a: \n", rank);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < i+1; j++) 
                cout << a[i][j] << " ";
            cout << endl;
        }
    }

    delete [] a, a[0], b, rc, ds;
    MPI_Finalize();
    return 0;
}
