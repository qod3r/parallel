#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;


void iToCombination(int* comb, int num, int base, int len) {
    int idx = 0;
    while (num > 0) {
        comb[len - 1 - idx] = num % base;
        // printf("num: %d, comb: %d\n", num, comb[idx]);
        idx++;
        num /= base;
    }
}

int main() {
    int comb[81];
    iToCombination(comb, 59000, 3, 81);
    for (int i = 0; i < 81; i++) {
        cout << comb[i] << " ";
    }

    return 0;
}