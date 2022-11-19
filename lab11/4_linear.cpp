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
    if (rank == 0)
        cin >> n;
    double t1, t2;
    double* v1 = new double[n];
    double* v2 = new double[n];
    for (int i = 0; i < n; i++) {
        v1[i] = 1.0 / (1.0 + i);
        v2[i] = i / (1.0 + i);
    }
    double prod = 0;
    t1 = MPI_Wtime();
    for (int i = 0; i < n; i++) {
        prod += v1[i] * v2[i];
    }
    t2 = MPI_Wtime();
    cout << prod << endl;
    cout << t2-t1 << " s" << endl;

    delete [] v1, v2;
    MPI_Finalize();
    return 0;
}