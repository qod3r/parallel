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

    int n = 1000000;
    double t1, t2;
    if (rank == 0) {
        int* a = new int[n];
        for (int i = 0; i < n; i++) a[i] = rank + 1;

        t1 = MPI_Wtime();
        // MPI_Send(&a[0], n, MPI_INT, 1, 777, MPI_COMM_WORLD);
        // MPI_Recv(&a[0], n, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        MPI_Sendrecv(&a[0], n, MPI_INT, 1, 777, &a[0], n, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        t2 = MPI_Wtime();
        cout << "elements: " << n << endl;
    }
    if (rank == 1) {
        int* b = new int[n];
        // int* bb = new int[n];
        for (int i = 0; i < n; i++) b[i] = rank + 1;

        t1 = MPI_Wtime();
        // MPI_Recv(&bb[0], n, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        // MPI_Send(&b[0], n, MPI_INT, 0, 777, MPI_COMM_WORLD);
        MPI_Sendrecv(&b[0], n, MPI_INT, 0, 777, &b[0], n, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        t2 = MPI_Wtime();
    }

    cout << "rank: " << rank << ", time: " << t2 - t1 << endl;

    MPI_Finalize();
    return 0;
}