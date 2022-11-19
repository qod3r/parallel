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

    int* a = new int[size * 2];
    if (rank == 0) {
        for (int i = 0; i < size * 2; i++) {
            a[i] = i;
        }
    }

    int* b = new int[2];

    MPI_Scatter(a, 2, MPI_INT, b, 2, MPI_INT, 0, MPI_COMM_WORLD);

    usleep(rank * 100000);
    printf("rank: %d, b: ", rank);
    for (int i = 0; i < 2; i++) {
        cout << b[i] << " ";
    }
    cout << endl;

    delete [] a, b;
    MPI_Finalize();
    return 0;
}
