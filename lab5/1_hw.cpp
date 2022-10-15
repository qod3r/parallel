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

    if (rank == 0) {
        int **a = new int*[n];
        a[0] = new int[n*n];
        for (int i = 1; i < n; i++)
            a[i] = a[i-1] + n;
        printf( "rank = %d, a: \n",rank );
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = 10 * (i+1) + (j+1);
                if (i == 0)
                    cout << a[i][j] << " ";
            }
        }
        cout << endl;

        for (int i = 1; i < size; i++) {
            MPI_Send(&a[i][0], n, MPI_INT, i, 777, MPI_COMM_WORLD);
        }
        delete []a[0];
        delete []a;
    }

    if (rank != 0) {
        int *b = new int[n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                b[i] = 0;
            }
        }
        usleep(100000 * rank);
        printf( "rank = %d, a: \n",rank );
        MPI_Recv(&b[0], n, MPI_INT, 0, 777, MPI_COMM_WORLD, &stat);
        for (int i = 0; i < n; i++) {
            cout << b[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
