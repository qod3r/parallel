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
        int n = 0;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&n, 1, MPI_INT, -2, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            a[stat.MPI_SOURCE] = n;
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
