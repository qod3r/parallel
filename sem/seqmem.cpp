#include <iostream>
#include <algorithm>
#include <chrono>


using namespace std;


double weight(double* a, int n) {
    double res = 0;
    for (int i = 0; i < n; i++)
        res += a[i];
    return res;
}

void diff(double* piles, int n, int m) {
    double mean = 0;
    for (int i = 0; i < m; i++)
        mean += piles[i];
    mean /= m;
    // cout << "mean: " << mean << endl;

    double d = 0;
    for (int i = 0; i < m; i++)
        d += abs(piles[i] - mean);
    d /= m;
    double diff = abs(piles[0]-piles[1]) + abs(piles[0]-piles[2]) + abs(piles[1]-piles[2]);
    diff /= m;
    cout << "diff: " << diff << endl;
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

double* greedy(double* rocks, double* piles, int n, int m) {
    // сортируем камни по убыванию
    sort(rocks, rocks + n, greater<double>());

    for (int i = 0; i < n; i++) {
        // берём самый тяжелый камень
        double rock = rocks[i];
        rocks[i] = -1;
        
        // находим самую легкую кучу
        int lightest = lightest_pile(piles, m);
        // суём туда камень
        piles[lightest] += rock;
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
        cout << a[i] << " ";
    }
    cout << endl;
    // кучи
    double* b = new double[m];
    for (int i = 0; i < m; i++)
        b[i] = 0;

    auto start = chrono::high_resolution_clock::now();
    b = greedy(a, b, n, m);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    for (int i = 0; i < m; i++) {
        cout << "pile " << i << ": ";
        cout << b[i] << endl;
    }
    diff(b, n, m);
    cout << "time: " << duration.count() << "μs" << endl;

    delete[] a, b;
    return 0;
}