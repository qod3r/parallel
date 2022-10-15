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

    // 10^6
    unsigned int n = 1000000;
    if (argc > 1) {
        n = strtoul(argv[1], nullptr, 10);
    }

    if (rank == 0) {
        double sum = 0;

        double t1 = MPI_Wtime();
        for (int i = 1; i < size; i++) {
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            double a = 0;
            MPI_Recv(&a, 1, MPI_DOUBLE, stat.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            sum += a;
        }
        double t2 = MPI_Wtime();
        double t = t2 - t1;
        
        // печать результата и затраченного времени
        cout << "sum of " << n << " elements: " << sum << endl;
        cout << "time elapsed: " << t << " s" << endl;
    } else {
        // определение диапазона расчетов для данного процесса
        int partStart = int(double(rank - 1) / double(size - 1) * n);
        int partEnd = int(double(rank) / double(size - 1) * n);
        cout << "rank: " << rank << ", start: " << partStart << ", end: " << partEnd << endl;

        double s = 0;
        // суммирование
        for (int i = partStart; i < partEnd; i++) {
            s += (1.0 / (i + 1.0));
        }

        // отправка суммы
        MPI_Send(&s, 1, MPI_DOUBLE, 0, 777, MPI_COMM_WORLD);

        // печать суммы этого процесса с задержкой 50мс
        usleep(50000 * rank);
        cout << "rank: " << rank << ", s: " << s << endl;
    }

    MPI_Finalize();
    return 0;
}