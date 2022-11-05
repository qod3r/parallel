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
    int* a = new int[asize];
    int* b = new int[rank + 1];
    for (int i = 0; i < rank + 1; i++) {
        b[i] = rank;
    }

    int* rc = new int[size];
    int* ds = new int[size];
    for (int i = 0; i < size; i++) {
        rc[i] = i + 1;
        ds[i] = i * (i + 1) / 2;
    }

    MPI_Gatherv(b, rank+1, MPI_INT, a, rc, ds, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Gather(b, 2, MPI_INT, a, 2, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Allgather(&rank, 1, MPI_INT, a, 1, MPI_INT, MPI_COMM_WORLD);

    int arrsize;
    int* arr;
    if (rank == 0) {
        arr = a;
        arrsize = asize;
    } else {
        arr = b;
        arrsize = rank + 1;
    }

    usleep(rank*100000);
    printf("rank: %d, a: ", rank);
    for (int i = 0; i < arrsize; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete [] a, b, rc, ds;
    MPI_Finalize();
    return 0;
}
