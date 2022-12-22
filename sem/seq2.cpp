#include <iostream>
#include <algorithm>
#include <limits>


using namespace std;


double weight(double* a, int n) {
    int res = 0;
    for (int i = 0; i < n; i++) {
        res += a[i];
    }
    return res;
}

int lightest_pile(double* w, int n) {
    double min = 2;
    int idx = 0;
    for (int i = 0; i < n; i++) {
        if (w[i] < min) {
            min = w[i];
            idx = i;
        }
        printf("%d: %f, min: %f, %d\n", i, w[i], min, idx);
    }
    cout << "idx: " << idx << endl;
    return idx;
}

double* greedy(double* rocks, double* piles, int n, int m) {
    sort(rocks, rocks + n, greater<double>());
    int counts[m];
    for (int i = 0; i < m; i++) {
        counts[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        double rock = rocks[i];
        rocks[i] = -1;

        int lightest = lightest_pile(piles, m);
        piles[lightest] += rock;
        counts[lightest]++;
    }

    return piles;
}

int main() {
    srand(0);
    // кол-во камней
    int n = 10;
    // кол-во куч
    int m = 3;

    // веса камней
    double* a = new double[n];
    for (int i = 0; i < n; i++) {
        a[i] = (double)(rand()+1) / (double)RAND_MAX;
        cout << a[i] << " ";
    }
    cout << endl;

    // кучи
    double* b = new double[m];
    for (int i = 0; i < m; i++) {
        b[i] = 0;
    }

    b = greedy(a, b, n, m);
    for (int i = 0; i < m; i++)
            cout << b[i] << " ";
    cout << endl;

    return 0;
}