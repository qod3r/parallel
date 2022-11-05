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
    if (rank == 0) cin >> n;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int **a = new int*[size];
    a[0] = new int[n*size];
    for (int i = 1; i < size; i++)
        a[i] = a[i-1] + n;

    int* b = new int[n];
    for (int i = 0; i < n; i++) {
        b[i] = rank;
    }

    // MPI_Gatherv(b, rank+1, MPI_INT, a, rc, ds, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(b, n, MPI_INT, a[0], n, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Allgather(&rank, 1, MPI_INT, a, 1, MPI_INT, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("rank: %d, a: \n", rank);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < n; j++) 
                cout << a[i][j] << " ";
            cout << endl;
        }
    }

    delete [] a, a[0], b;
    MPI_Finalize();
    return 0;
}
