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
    int* b = new int[2];
    b[0] = rank, b[1] = rank;

    MPI_Gather(b, 2, MPI_INT, a, 2, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Allgather(&rank, 1, MPI_INT, a, 1, MPI_INT, MPI_COMM_WORLD);

    if (rank == 0) {
        // usleep(rank*100000);
        printf("rank: %d, a: ", rank);
        for (int i = 0; i < size * 2; i++) {
            cout << a[i] << " ";
        }
        cout << endl;
    }


    delete []a;
    delete []b;
    MPI_Finalize();
    return 0;
}