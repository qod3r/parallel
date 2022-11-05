#include <iostream>
#include "mpi.h"
#include <unistd.h>


using namespace std;

int nextDest(int currRank, int size) {
    int nextDest = 0;

    if (currRank + 2 < size) {
        nextDest = currRank + 2;
    } else {
        if (currRank % 2 == 0) {
            nextDest = 0;
        } else {
            nextDest = 1;
        }
    }

    return nextDest;
}

int main(int argc, char *argv[]) {
    int rank;
    int size;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = 0;
    if (rank == 0) data = 22;
    if (rank == 1) data = 11;

    int cycle = 0;
    while (true) {
        if (rank == 0) {
            sleep(1);
            cout << endl << "cycle " << cycle << endl;
            cout << "r  d" << endl;
        } else {
            sleep(2);
        }
        cout << rank << "  " << data << endl;

        int dest = 0;
        dest = nextDest(rank, size);
        MPI_Sendrecv(&data, 1, MPI_INT, dest, 777, &data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        
        cycle++;
    }

    MPI_Finalize();
    return 0;
}