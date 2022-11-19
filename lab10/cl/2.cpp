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
    if (rank == 0) {
        cin >> n;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int** a = new int*[size];
    a[0] = new int[size * n];
    for (int i = 1; i < size; i++)
        a[i] = a[i-1] + n;
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = 10 * (i + 1) + j + 1;
            }
        }
    }

    int* b = new int[n];
    MPI_Scatter(a[0], n, MPI_INT, b, n, MPI_INT, 0, MPI_COMM_WORLD);

    usleep(rank * 100000);
    printf("rank: %d, a: ", rank);
    for (int i = 0; i < n; i++) {
        cout << b[i] << " ";
    }
    cout << endl;

    MPI_Finalize();
    return 0;
}
