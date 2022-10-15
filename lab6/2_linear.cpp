#include <iostream>
#include "mpi.h"


using namespace std;

int main(int argc, char* argv[]) {
    int rank;
    int size;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // 10^6
    unsigned int n = 1000000;
    if (argc > 1) {
        n = strtoul(argv[1], nullptr, 10);
    }

    double sum = 0;
    double t1 = MPI_Wtime();
    for (int i = 0; i < n; i++) {
        sum += (1.0 / (i + 1.0));
    }
    double t2 = MPI_Wtime();
    double t = t2 - t1;

    cout << "sum of " << n << " elements: " << sum << endl;
    cout << "time elapsed: " << t << " s" << endl;

    return 0;
}