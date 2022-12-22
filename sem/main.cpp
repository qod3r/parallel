#include <iostream>
#include "mpi.h"
#include <unistd.h>
#include <math.h>
#include <bits/stdc++.h>

using namespace std;


void iToCombination(int* comb, int num, int base, int len) {
    int idx = 0;
    while (num > 0) {
        comb[len - 1 - idx] = num % base;
        idx++;
        num /= base;
    }
}

void bestResults(int* res, double* a, int n, int m, int len) {
    double* piles = new double[m];
    for (int i = 0; i < m; i++)
        piles[i] = 0;

    int* comb = new int[n];
    for (int i = 0; i < n; i++)
        comb[i] = 0;

    double min = INT32_MAX - 1.0;
    double diff = 0;
    double mean = 0;
    int best_comb = 0;

    for (int i = 0; i < len; i++) {
        iToCombination(comb, res[i], m, n);

        // кладем камни в кучи
        for (int stone = 0; stone < n; stone++) {
            piles[comb[stone]] += a[stone];
            comb[stone] = 0;
        }

        diff = abs(piles[0]-piles[1]) + abs(piles[0]-piles[2]) + abs(piles[1]-piles[2]);
        diff /= m;

        for (int j = 0; j < m; j++)
            piles[j] = 0;
        if (diff < min) {
            min = diff;
            best_comb = res[i];
        }
    }
    iToCombination(comb, best_comb, m, n);
    for (int stone = 0; stone < n; stone++)
        piles[comb[stone]] += a[stone];

    cout << "piles: [";
    for (int i = 0; i < m; i++)
        cout << piles[i] << " ";
    cout << "\b]\n";
    printf("diff: %f\n", diff);
}

int main(int argc, char *argv[]) {
    int rank;
    int size;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // кол-во камней
    int n = 10;
    // кол-во куч
    int m = 3;

    if (rank == 0) {
        cout << "n, m: ";
        cin >> n >> m;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // веса камней
    double* a = new double[n];
    // кучи
    double* piles = new double[m];
    for (int i = 0; i < m; i++)
        piles[i] = 0;
    double time_;
    if (rank == 0) {
        srand(0);
        // (0, 1]
        for (int i = 0; i < n; i++) {
            a[i] = (double)(rand()+1) / (double)RAND_MAX;
            cout << a[i] << " ";
        }
        cout << endl;
        time_ = MPI_Wtime();
    }
    MPI_Bcast(a, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // текущая комбинация
    int* comb = new int[n];
    for (int i = 0; i < n; i++)
        comb[i] = 0;

    double min = INT32_MAX - 1.0;
    double diff = 0;
    double mean = 0;
    int best_comb = 0;

    // перестановки
    for (int i = rank; i < pow(m, n); i += size) {
        iToCombination(comb, i, m, n);
        // кладем камни в кучи
        for (int stone = 0; stone < n; stone++) {
            piles[comb[stone]] += a[stone];
            comb[stone] = 0;
        }

        // считаем отклонение
        // for (int j = 0; j < m; j++)
        //     mean += piles[j];
        // mean /= m;
        diff = abs(piles[0]-piles[1]) + abs(piles[0]-piles[2]) + abs(piles[1]-piles[2]);
        diff /= m;

        for (int j = 0; j < m; j++) {
            // diff += abs(piles[j] - mean);
            piles[j] = 0;
        }
        // сравниваем с минимумом
        if (diff < min) {
            min = diff;
            best_comb = i;
        }
    }
    
    int* proc_best = new int[size];
    MPI_Gather(&best_comb, 1, MPI_INT, proc_best, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        time_ = MPI_Wtime() - time_;
        for (int i = 0; i < size; i++) {
            cout << proc_best[i] << " ";
        }
        cout << endl;
        bestResults(proc_best, a, n, m, size);
        cout << "time: " << time_ * 1000000 << "μs" << endl;
    }

    delete[] a, piles, comb, proc_best;
    MPI_Finalize();
    return 0;
}