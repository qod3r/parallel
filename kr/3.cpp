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

    int n = rank + 10;
    int *s = new int[size];
    MPI_Gather(&n, 1, MPI_INT, s, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("rank: %d, s: ", rank);
        for (int i = 0; i < size; i++) {
            cout << s[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}