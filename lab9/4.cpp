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

    int** a = new int*[2 * size];
    a[0] = new int[2 * size * size];
    for (int i = 1; i < 2 * size; i++)
        a[i] = a[i-1] + size;
    for (int i = 0; i < 2 * size; i++)
        for (int j = 0; j < size; j++)
            a[i][j] = 0;

    int** b = new int*[2];
    b[0] = new int[2 * (rank + 1)];
    b[1] = b[0] + rank+1;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < rank+1; j++)
            b[i][j] = rank;

    int* rc = new int[size];
    int* ds = new int[size];
    for (int i = 0; i < size; i++) {
        rc[i] = i + 1;
        ds[i] = i * size * 2;
    }
        
    MPI_Gatherv(b[0], rank+1, MPI_INT, a[0], rc, ds, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gatherv(b[1], rank+1, MPI_INT, a[1], rc, ds, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("rank: %d, a:\n", rank);
        for (int i = 0; i < 2 * size; i++) {
            for (int j = 0; j < size; j++)
                cout << a[i][j] << " ";
            cout << endl;
        }
    }

    delete [] a[0], b[0], a, b, rc, ds;

    MPI_Finalize();
    return 0;
}