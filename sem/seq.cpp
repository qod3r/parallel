#include <iostream>
#include <algorithm>
#include <chrono>


using namespace std;


double weight(double* a, int n) {
    double res = 0;
    for (int i = 0; i < n; i++) {
        res += a[i];
    }
    return res;
}

void diff(double** piles, int n, int m) {
    double mean = 0;
    for (int i = 0; i < m; i++) {
        mean += weight(piles[i], n);
    }
    mean /= m;
    cout << "mean: " << mean << endl;

    double d = 0;
    for (int i = 0; i < m; i++) {
        d += abs(weight(piles[i], n) - mean);
    }
    d /= m;
    cout << "d: " << d << endl;
}

int lightest_pile(double* w, int n) {
    double min = numeric_limits<double>::max();
    int idx = 0;
    for (int i = 0; i < n; i++) {
        if (w[i] < min) {
            min = w[i];
            idx = i;
        }
        // printf("%d: %f, min: %f, %d\n", i, w[i], min, idx);
    }
    return idx;
}

double** greedy(double* rocks, double** piles, int n, int m) {
    // сортируем камни по убыванию
    sort(rocks, rocks + n, greater<double>());
    double weights[m];
    int counts[m];
    for (int i = 0; i < m; i++) {
        weights[i] = 0;
        counts[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        // берём самый тяжелый камень
        double rock = rocks[i];
        rocks[i] = -1;
        
        // находим самую легкую кучу
        int lightest = lightest_pile(weights, m);
        // суём туда камень
        piles[lightest][counts[lightest]] = rock;
        // следим за счетчиками
        counts[lightest]++;
        weights[lightest] += rock;
    }

    return piles;
}

int main() {
    srand(0);
    // кол-во камней
    int n = 10;
    // кол-во куч
    int m = 3;

    cout << "n, m: ";
    cin >> n >> m;

    // веса камней
    double* a = new double[n];
    for (int i = 0; i < n; i++) {
        a[i] = (double)(rand()+1) / (double)RAND_MAX;
        // cout << a[i] << " ";
    }
    cout << endl;

    // кучи
    double** b = new double*[m];
    for (int i = 0; i < m; i++) {
        b[i] = new double[n];
        for (int j = 0; j < n; j++) {
            b[i][j] = 0;
        }
    }

    auto start = chrono::high_resolution_clock::now();
    b = greedy(a, b, n, m);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    for (int i = 0; i < m; i++) {
        cout << "pile " << i << ": ";
        cout << weight(b[i], n) << endl;
        // cout << ", [";
        // for (int j = 0; j < n; j++) {
        //     if (b[i][j] != 0)
        //         cout << b[i][j] << " ";
        // }
        // cout << "\b]\n";
    }
    diff(b, n, m);
    cout << "time: " << duration.count() << "μs" << endl;

    for (int i = 0; i < m; i++)
        delete[] b[i];
    delete[] a, b;
    return 0;
}