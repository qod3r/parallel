#include <iostream>
#include "mpi.h"
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    int rank;
    int size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;
    if (rank == 0)
        cin >> n;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int** a = new int*[n];
    a[0] = new int[n * n];
    for (int i = 1; i < n; i++)
        a[i] = a[i-1] + n;
    if (rank == 0)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                a[i][j] = 10 * (i + 1) + j + 1;

    int thisSize = n / size;
    // если есть остаток от деления
    // [0, n % size) процессы получают на 1 строку больше
    if (rank < n % size)
        thisSize++;
    int* sc = new int[size];
    // сбор кол-ва строк в sc каждого процесса
    MPI_Allgather(&thisSize, 1, MPI_INT, sc, 1, MPI_INT, MPI_COMM_WORLD);
    // превращение кол-ва строк в кол-во элементов
    for (int i = 0; i < size; i++)
        sc[i] *= n;

    int** b = new int*[thisSize];
    b[0] = new int[thisSize * n];
    for (int i = 1; i < thisSize; i++)
        b[i] = b[i-1] + n;

    int* ds = new int[size];
    ds[0] = 0;
    for (int i = 1; i < size; i++) {
        ds[i] = ds[i-1] + sc[i-1];
    }

    MPI_Scatterv(a[0], sc, ds, MPI_INT, b[0], thisSize * n, MPI_INT, 0, MPI_COMM_WORLD);

    usleep(rank * 100000);
    printf("rank: %d, b: \n", rank);
    for (int i = 0; i < thisSize; i++) {
        for (int j = 0; j < n; j++) {
            cout << b[i][j] << " ";
        }
        cout << endl;
    }

    delete [] a[0], a, b[0], b, sc, ds;
    MPI_Finalize();
    return 0;
}
