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
    srand(time(0) * rank);

    if (rank == 0) {
        int *a = new int[size];
        a[0] = 0;

        for (int i = 1; i < size; i++) {
            MPI_Probe(-2, -1, MPI_COMM_WORLD, &stat);
            MPI_Recv(&a[stat.MPI_SOURCE], 1, MPI_INT, stat.MPI_SOURCE, -1, MPI_COMM_WORLD, &stat);
        }

        cout << "a: ";
        for (int i = 0; i < size; i++) {
            cout << a[i] << " ";
        }
        cout << endl;

        delete []a;
    } else {
        int a = random();
        MPI_Send(&rank, 1, MPI_INT, 0, 777, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
