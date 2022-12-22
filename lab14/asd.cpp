#include <iostream>
#include <stdio.h>
#include "mpi.h"
#include "string.h"

using namespace std;

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int a;
    double b;
    char *c = new char[12];

    int bl[] = {1, 1, 12};
    MPI_Datatype mt;
    MPI_Datatype dt[] = {MPI_INT, MPI_DOUBLE, MPI_CHAR};
    MPI_Aint ds[3], aa, ab, ac;

    MPI_Get_address(&a, &aa);
    MPI_Get_address(&b, &ab);
    MPI_Get_address(c, &ac);

    ds[0] = 0;
    ds[1] = ab - aa;
    ds[2] = ac - aa;


    MPI_Type_struct(3, bl, ds, dt, &mt);
    MPI_Type_commit(&mt);

    if (rank == 0) {
        cout << aa << " " << ab << " " << ac << endl;
        a = 7;
        b = 0.8;
        strcpy(c, "hello world!");
        MPI_Send(&a, 1, mt, 1, 777, MPI_COMM_WORLD);
    }
    if (rank == 1) {
        MPI_Recv(&a, 1, mt, 0, 777, MPI_COMM_WORLD, &stat);
        printf("a= %d \n", a);
        printf("b= %f \n", b);
        printf("c= %s \n", c);
    }

    MPI_Finalize();
    return 0;
}