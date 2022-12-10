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

    MPI_Datatype mt;

    int* a = new int[n];
    for (int i = 0; i < n; i++) a[i] = -1;
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            a[i] = (i % 2 == 0) ? 1 : 0;
        }
        double time = MPI_Wtime();
        MPI_Type_vector(n/2, 1, 2, MPI_INT, &mt);
        MPI_Type_commit(&mt);
        MPI_Send(a, 1, mt, 1, 777, MPI_COMM_WORLD);
        MPI_Type_free(&mt);

        MPI_Type_vector(n/2, 1, 2, MPI_INT, &mt);
        MPI_Type_commit(&mt);
        MPI_Recv(a, 1, mt, 1, 777, MPI_COMM_WORLD, &stat);
        MPI_Type_free(&mt);


        time = MPI_Wtime() - time;
        cout << time << " s" << endl;
    } else {
        MPI_Type_vector(n/2, 1, 2, MPI_INT, &mt);
        MPI_Type_commit(&mt);
        MPI_Recv(a, 1, mt, 0, 777, MPI_COMM_WORLD, &stat);
        MPI_Type_free(&mt);

        MPI_Type_vector(n/2, 1, 2, MPI_INT, &mt);
        MPI_Type_commit(&mt);
        MPI_Send(a, 1, mt, 0, 777, MPI_COMM_WORLD);
        MPI_Type_free(&mt);
    }
    
    delete [] a;
    MPI_Finalize();
    return 0;
}