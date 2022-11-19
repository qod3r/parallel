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

    int** a = new int*[size * 2];
    a[0] = new int[size * 2 * size];
    for (int i = 1; i < size * 2; i++)
        a[i] = a[i-1] + size;
    if (rank == 0)
        for (int i = 0; i < size * 2; i++)
            for (int j = 0; j < size; j++)
                a[i][j] = i / 2;

    int** b = new int*[2];
    b[0] = new int[2 * (rank + 1)];
    for (int i = 1; i < 2; i++)
        b[i] = b[i-1] + (rank + 1);

    int* sc = new int[size];
    int* ds = new int[size];
    for (int i = 0; i < size; i++) {
        sc[i] = (i + 1) * 2;
        ds[i] = size * 2 * i;
    }

    MPI_Scatterv(a[0], sc, ds, MPI_INT, b[0], (rank + 1) * 2, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("a: \n");
        for (int i = 0; i < size * 2; i++) {
            for (int j = 0; j < size; j++) {
                cout << a[i][j] << " ";
            }
            cout << endl;
        }
    }
    usleep((rank + 1) * 100000);
    printf("rank: %d, b: \n", rank);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < rank + 1; j++) {
            cout << b[i][j] << " ";
        }
        cout << endl;
    }

    delete [] a[0], a, b[0], b, sc, ds;
    MPI_Finalize();
    return 0;
}
