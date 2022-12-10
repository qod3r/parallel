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

    MPI_Datatype mt;
    MPI_Type_vector(20/3 + 1, 1, 3, MPI_INT, &mt);
    MPI_Type_commit(&mt);

    int* a = new int[20];
    if (rank == 0) {
        for (int i = 0; i < 20; i++)
            a[i] = i;

        MPI_Send(a, 1, mt, 1, 777, MPI_COMM_WORLD);
    } else {
        for (int i = 0; i < 20; i++)
            a[i] = -1;
        MPI_Recv(a, 1, mt, 0, 777, MPI_COMM_WORLD, &stat);
    }

    usleep(rank * 100000);
    printf("rank: %d, a: ", rank);
    for (int i = 0; i < 20; i++)
        cout << a[i] << " ";
    cout << endl;

    delete [] a;
    MPI_Finalize();
    return 0;
}