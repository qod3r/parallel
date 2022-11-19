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

    int asize = size * (size + 1) / 2;
    int** a = new int*[asize];
    int n;
    if (rank == 0) {
        cin >> n;

        a[0] = new int[asize * n];
        for (int i = 1; i < asize; i++)
            a[i] = a[i-1] + n;

        for (int i = 0; i < asize; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = 10 * (i + 1) + j + 1;
            }
        }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int** b = new int*[rank + 1];
    b[0] = new int[(rank + 1) * n];
    for (int i = 1; i < rank + 1; i++) {
        b[i] = b[i-1] + n;
    }

    int* sc = new int[size];
    int* ds = new int[size];
    for (int i = 0; i < size; i++) {
        sc[i] = (i + 1) * n;
        ds[i] = i * (i + 1) / 2 * n;
    }

    MPI_Scatterv(a[0], sc, ds, MPI_INT, b[0], (rank + 1) * n, MPI_INT, 0, MPI_COMM_WORLD);

    usleep(rank * 100000);
    printf("rank: %d, b:\n", rank);
    for (int i = 0; i < rank + 1; i++) {
        for (int j = 0; j < n; j++)
            cout << b[i][j] << " ";
        cout << endl;
    }

    delete [] a[0], b[0];
    delete [] a, b, sc, ds;
    MPI_Finalize();
    return 0;
}
