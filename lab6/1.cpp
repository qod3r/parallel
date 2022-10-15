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
    srand(time(0) * (rank + 1));

    if (rank == 0) {
        // создание двумерного массива и заполнение нулями
        int **a = new int*[size];
        a[0] = new int[size*size];
        for (int i = 1; i < size; i++)
            a[i] = a[i-1] + size;

        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                a[i][j] = 0;


        for (int i = 1; i < size; i++) {
            // получение информации о следующем отправителе
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            cout << "from: " << stat.MPI_SOURCE << ", ";

            // получение информации о количестве входящих элементов
            int n = 0;
            MPI_Get_count(&stat, MPI_INT, &n);
            cout << "count: " << n << endl;

            // получение элементов в rank-ую строку массива a
            MPI_Recv(&a[stat.MPI_SOURCE][0], n, MPI_INT, stat.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        }
        
        // печать результата
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
        // получение случайного числа от 1 до size
        int n = 0;
        while (n == 0) {
            n = rand() % size;
        }

        // заполнение массива
        int *a = new int[n];
        for (int i = 0; i < n; i++) {
            a[i] = rank;
        }

        // отправка массива
        MPI_Send(&a[0], n, MPI_INT, 0, 777, MPI_COMM_WORLD);

        // печать массива этого процесса с задержкой 50мс
        usleep(50000 * rank);
        cout << "rank: " << rank << ", a: ";
        for (int i = 0; i < n; i++) {
            cout << a[i] << " ";
        }
        cout << endl;

        delete []a;
    }

    MPI_Finalize();
    return 0;
}
