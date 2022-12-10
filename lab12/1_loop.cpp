#include <iostream>
#include "mpi.h"
#include <unistd.h>
#include <math.h>

using namespace std;

int main(int argc, char *argv[]) {
    int rank;
    int size;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;
    if (rank == 0) {
        cout << "10^";
        cin >> n;
        n = pow(10, n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int* a = new int[n];
    for (int i = 0; i < n; i++) a[i] = -1;
    if (rank == 0) {
        for (int i = 0; i < n; i++)
            a[i] = (i % 2 == 0) ? 1 : 0;
        double time = MPI_Wtime();
        for (int i = 0; i < n; i += 2)
            MPI_Send(a+i, 1, MPI_INT, 1, 777, MPI_COMM_WORLD);
        for (int i = 0; i < n; i += 2)
            MPI_Recv(a+i, 1, MPI_INT, 1, 777, MPI_COMM_WORLD, &stat);
        time = MPI_Wtime() - time;
        cout << time << " s" << endl;
    } else {
        for (int i = 0; i < n; i += 2)
            MPI_Recv(a+i, 1, MPI_INT, 0, 777, MPI_COMM_WORLD, &stat);
        for (int i = 0; i < n; i += 2)
            MPI_Send(a+i, 1, MPI_INT, 0, 777, MPI_COMM_WORLD);
    }

    delete [] a;
    MPI_Finalize();
    return 0;
}