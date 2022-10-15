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
    int n = size;

    if (rank != 0)
    {
        int *a = new int[n];

        for (int i = 0; i < n; i++) {
            a[i] = rank;
        }
        MPI_Send(&a[0], n, MPI_INT, 0, 777, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        int **a = new int*[n];
        a[0] = new int[n*n];
        for (int i = 1; i < n; i++)
            a[i] = a[i-1] + n;
            
        for (int i = 0; i < n; i++) {
            a[0][i] = 0;
        }

        for (int i = 1; i < n; i++) {
            MPI_Recv(&a[i][0], n, MPI_INT, i, 777, MPI_COMM_WORLD, &stat);
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << a[i][j] << " ";
            cout<<endl;
        }
    }

    MPI_Finalize();
    return 0;
}
