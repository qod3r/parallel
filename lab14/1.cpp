#include <iostream>
#include "mpi.h"
#include <unistd.h>
#include <string.h>

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
        cin >> n;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int* a = new int[n];
    double* b = new double[n];
    char* h = new char[12];

    int bl[] = {n, n, 12};
    MPI_Datatype mt;
    MPI_Datatype dt[] = {MPI_INT, MPI_DOUBLE, MPI_CHAR};
    MPI_Aint ds[3], aa, ab, ah;
    MPI_Get_address(a, &aa);
    MPI_Get_address(b, &ab);
    MPI_Get_address(h, &ah);
    ds[0] = 0;
    ds[1] = ab - aa;
    ds[2] = ah - aa;

    MPI_Type_struct(3, bl, ds, dt, &mt);
    MPI_Type_commit(&mt);

    if (rank == 0) {
        cout << ds[0] << " " << ds[1] << " " << ds[2] << endl;
        // h[0] = '\0';
        for (int i = 0; i < n; i++) {
            a[i] = i;
            b[i] = i / (i + 1);
            // strcat(h, "a");
        }
        strcpy(h, "hello world!");
        MPI_Send(&a, 1, mt, 1, 777, MPI_COMM_WORLD);
    } else {
        cout << ds[0] << " " << ds[1] << " " << ds[2] << endl;
        MPI_Recv(&a, 1, mt, 0, 777, MPI_COMM_WORLD, &stat);
        printf("a: [");
        for (int i = 0; i < n; i++)
            cout << a[i] << " ";
        printf("\b]\nb: [");
        for (int i = 0; i < n; i++)
            cout << b[i] << " ";
        printf("\b]\nh: %s\n", h);
    }

    delete[] a, b, h;
    MPI_Finalize();
    return 0;
}