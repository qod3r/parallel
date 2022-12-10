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

    int *a = new int[10];
    for (int i = 0; i < 10; i++)
        a[i] = rank;

    if (rank != 0) {
        MPI_Send(a, 10, MPI_INT, 0, 777, MPI_COMM_WORLD);
    } else {
        int* s = new int[size];
        for (int i = 0; i < size; i++)
            s[i] = 0;

        for (int i = 1; i < size; i++) {
            MPI_Recv(a, 10, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            for (int j = 0; j < 10; j++)
                s[i] += a[j];
        }

        printf("rank: %d, s: ", rank);
        for (int i = 0; i < size; i++) {
            cout << s[i] << " ";
        }
        cout << endl;
        delete [] s;
    }

    delete [] a;
    MPI_Finalize();
    return 0;
}