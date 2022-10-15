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

    int n = 0;
    if (rank == 0) {
        // cout << "source: " << MPI_ANY_SOURCE << endl;
        // cout << "tag: " << MPI_ANY_TAG << endl;
        // cout << "int: " << MPI_INT << endl;

        for (int i = 1; i < size; i++) {
            MPI_Recv(&n, 1, MPI_INT, -2, -1, MPI_COMM_WORLD, &stat);
            cout << n << " " << stat.MPI_SOURCE << endl;
        }
    } else {
        MPI_Send(&rank, 1, MPI_INT, 0, 777, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
