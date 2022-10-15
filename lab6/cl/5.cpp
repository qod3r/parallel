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
    // srand(time(0) * rank);

    if (rank == 0) {
        int **a = new int*[size];
        a[0] = new int[size*size];
        for (int i = 1; i < size; i++) {
            a[i] = a[i-1] + size;
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++)
                a[i][j] = 0;
        }

        for (int i = 1; i < size; i++) {
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            cout << "probed " << stat.MPI_SOURCE << ", ";
            int n = 0;
            MPI_Get_count(&stat, MPI_INT, &n);
            cout << "count: " << n << ", ";
            cout << "receiving..." << endl;
            MPI_Recv(&a[stat.MPI_SOURCE][0], n, MPI_INT, stat.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            cout << "received" << endl;
        }

        cout << "a:\n";
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << a[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;

        delete []a[0];
        delete []a;
    } else {
        int *a = new int[rank + 1];
        for (int i = 0; i < rank + 1; i++) {
            a[i] = rank;
        }
        MPI_Send(&a[0], rank+1, MPI_INT, 0, 777, MPI_COMM_WORLD);

        delete []a;
    }

    MPI_Finalize();
    return 0;
}
