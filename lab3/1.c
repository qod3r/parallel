#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "mpi/mpi.h"

using namespace std;

int main(int argc, char *argv[]) {
    int rank;
    int size;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int *a = new int[3 * size];
        int *b = new int[3];

        for (int i = 0; i < 3 * size; i++) {
            a[i] = i;
            if (i % 3 == 0) {
                b[i/3] = i/3*size;
            }
        }
        for (int i = 1; i < size; i++) {
            for (int j = 0; j < size; j++) {
                MPI_Send(&a[i + j*size], 1, MPI_INT, i, 777, MPI_COMM_WORLD);
            }
        }

        printf("rank=%d b: ", rank);
        for (int i = 0; i < 3; i++) cout << b[i] << " ";
        cout << endl;
        delete[] a;
    } else {
        int *b = new int[3];
        for (int i = 0; i < 3; i++) {        
            MPI_Recv(&b[i], 1, MPI_INT, 0, 777, MPI_COMM_WORLD, &stat);
        }

        usleep(rank * 100000);
        printf("rank=%d b: ", rank);
        for (int i = 0; i < 3; i++) {
            cout << b[i] << " ";
        }
        cout << endl;
        delete[] b;
    }
    MPI_Finalize();

    return 0;
}
