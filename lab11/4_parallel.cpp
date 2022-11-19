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
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int start = rank * n / size;
    int end = (rank+1) * n / size;

    double* v1 = new double[n/size];
    double* v2 = new double[n/size];
    int idx = 0;
    for (int i = start; i < end; i++) {
        v1[idx] = 1.0 / (1.0 + i);
        v2[idx] = i / (1.0 + i);
        idx++;
    }
    double prod = 0;
    double time = MPI_Wtime();
    for (int i = 0; i < n/size; i++) {
        prod += v1[i] * v2[i];
    }
    double prodsum;
    MPI_Reduce(&prod, &prodsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    time = MPI_Wtime() - time;

    if (rank == 0) {
        printf("prod: %f, time: %f\n", prodsum, time);
    }

    delete [] v1, v2;
    MPI_Finalize();
    return 0;
}