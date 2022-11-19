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

    int* a = new int[size];
    if (rank == 0)
        for (int i = 0; i < size; i++)
            a[i] = i;
    
    int* b = new int[rank + 1];
    int* sc = new int[size];
    int* ds = new int[size];
    for (int i = 0; i < size; i++) {
        sc[i] = i + 1;
        ds[i] = 0;
    }

    MPI_Scatterv(a, sc, ds, MPI_INT, b, rank+1, MPI_INT, 0, MPI_COMM_WORLD);

    usleep(rank * 100000);
    printf("rank: %d, b: ", rank);
    for (int i = 0; i < rank + 1; i++) {
        cout << b[i] << " ";
    }
    cout << endl;

    delete [] a, b, sc, ds;
    MPI_Finalize();
    return 0;
}
