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
    if (rank == 0)
        cin >> n;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Datatype mt;
    MPI_Type_vector(n/2, n, 2*n, MPI_INT, &mt);
    MPI_Type_commit(&mt);

    int** a = new int*[n];
    a[0] = new int[n * n];
    for (int i = 1; i < n; i++)
        a[i] = a[i-1] + n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (rank == 0) {
                a[i][j] = 10 * (i + 1) + j + 1;
            } else {
                a[i][j] = -1;
            }
        }
    }

    if (rank == 0) {
        MPI_Send(a[0], 1, mt, 1, 777, MPI_COMM_WORLD);
    } else {
        MPI_Recv(a[0], 1, mt, 0, 777, MPI_COMM_WORLD, &stat);
    }
    usleep(rank * 100000);
    printf("rank: %d, a: \n", rank);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
    
    delete [] a[0], a;
    MPI_Finalize();
    return 0;
}
