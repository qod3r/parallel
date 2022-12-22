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

    // 1) на 0-м процессе задается квадратная матрица NxN по формуле: a(i,j)=10*(i+1)+(j+1).
    // 2) нижнетреугольная матрица передается с 0-го процесса на 1-й.
    // 3) 1-й процесс сохраняет полученную матрицу в транспонированном виде (должна получиться верхнетреугольная матрица).

    int n;
    if (rank == 0)
        cin >> n;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int** a = new int*[n];
    a[0] = new int[n * n];
    for (int i = 1; i < n; i++)
        a[i] = a[i-1] + n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (rank == 0)
                a[i][j] = 10*(i+1) + (j+1);
            else
                a[i][j] = 0;
        }
    }

    int* bl = new int[n];
    int* ds = new int[n];
    for (int i = 0; i < n; i++) {
        bl[i] = i + 1;
        ds[i] = i * n;
    }
    MPI_Datatype lower_tg, cols;
    MPI_Type_indexed(n, bl, ds, MPI_INT, &lower_tg);
    MPI_Type_commit(&lower_tg);
    MPI_Type_create_hvector(n, 1, 4, lower_tg, &cols);
    MPI_Type_commit(&cols);

    if (rank == 0) {
        MPI_Send(a[0], 1, lower_tg, 1, 777, MPI_COMM_WORLD);
    } else {
        MPI_Recv(a[0], n*n, MPI_INT, 0, 777, MPI_COMM_WORLD, &stat);
        // MPI_Recv(a[0], 1, lower_tg, 0, 777, MPI_COMM_WORLD, &stat);
    }

    usleep(rank*100000);
    printf("rank: %d, a:\n", rank);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << a[i][j] << "\t\b\b";
        cout << endl;
    }

    delete [] a[0], a, bl, ds;
    MPI_Finalize();
    return 0;
}