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

    int n, len;
    char* name = new char[50];
    MPI_Get_processor_name(name, &len);
    if (rank == 0) {
        cin >> n;
        MPI_Send(&n, 1, MPI_INT, 3, 777, MPI_COMM_WORLD);
        int m;
        MPI_Recv(&m, 1, MPI_INT, 3, 777, MPI_COMM_WORLD, &stat);
        printf("name: %s, rank: %d, m: %d\n", name, rank, m);
    } else if (rank == 3) {
        MPI_Recv(&n, 1, MPI_INT, 0, 777, MPI_COMM_WORLD, &stat);
        printf("name: %s, rank: %d, n: %d\n", name, rank, n);
        MPI_Send(&n, 1, MPI_INT, 0, 777, MPI_COMM_WORLD);
    }

    delete [] name;
    MPI_Finalize();
    return 0;
}