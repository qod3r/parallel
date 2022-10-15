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
    int n = size*3;

    int **a = new int*[n];
    a[0] = new int[n*n];
    for (int i = 1; i < n; i++)
        a[i] = a[i-1] + n;

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = 10 * (i+1) + (j+1);
            }
        }

        for (int i = 1; i < size; i++) {
            MPI_Send(&a[i*3][0], 3*n, MPI_INT, i, 777, MPI_COMM_WORLD);
        }
    }

    if (rank != 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = 0;
            }
        }
        MPI_Recv(&a[0][0], 3*n, MPI_INT, 0, 777, MPI_COMM_WORLD, &stat);
    }

    usleep(100000 * rank);
    printf( "rank = %d, a: \n",rank );
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << a[i][j] << " ";
        cout<<endl;
    }

    delete []a[0];
    delete []a;

    MPI_Finalize();
    return 0;
}
