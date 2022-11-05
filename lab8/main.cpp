#include <iostream>
#include "mpi.h"
#include <unistd.h>
#include <math.h>

using namespace std;
int getRank(int size, int rank, int stage, bool send) {
    int powstage = int(pow(2, stage));

    if (send) {
        return ((rank + powstage) % size);
    } else {
        if (rank - powstage >= 0) {
            return rank - powstage;
        } else {
            return size + rank - powstage;
        }
    }
}

int Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
    int relative_rank = (rank - root + size) % size;
    
    int total_stages = int(ceil(log2(float(size))));
    for (int stage = 0; stage < total_stages; stage++) {
        if (relative_rank >= pow(2, stage) && relative_rank < pow(2, stage+1)) {
            /* receive from (rank - 2^stage) */
            MPI_Status stat;
            int from = getRank(size, rank, stage, false);
            // printf("recv %d <-- %d\n", rank, from);
            MPI_Recv(buffer, count, datatype, from, MPI_ANY_TAG, comm, &stat);
        }
        else if (relative_rank < pow(2, stage)) {
            /* send to (rank + 2^stage) */
            int to = getRank(size, rank, stage, true);
            // printf("send %d --> %d\n", rank, to);
            MPI_Send(buffer, count, datatype, to, 777, comm);
        }
    }
    return MPI_SUCCESS;
}

int main(int argc, char *argv[]) {
    int rank;
    int size;
    int n=0, i, s=0;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        n = 10000;
    }
    Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *a = new int[n];
    if (rank == 0)
        for(i = 0; i < n; i++)
            a[i] = i;
    double t1 = MPI_Wtime();
    // Bcast(a, n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(a, n, MPI_INT, 0, MPI_COMM_WORLD);
    double t2 = MPI_Wtime();

    // usleep((rank+1)*100000);
    // printf("rank: %d, n: %d, time: %f\n", rank, n, t2-t1);
    printf("rank: %d, a[9999]: %d, time: %f\n", rank, a[9999], t2 - t1);

    MPI_Finalize();
    return 0;
}