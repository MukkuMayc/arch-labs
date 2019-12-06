//
// Created by sergo on 12/5/19.
//
#include <iostream>
#include "matrix.hpp"
#include <chrono>

using namespace std;

int maxFromMinsParallel(Matrix<int> &A) {
    int size = A.getWidth();
    int rowMin[size];
    int i, j;
    int max = 0;
#pragma omp parallel for default(shared), private(i, j), reduction(max:max)
    for (i = 0; i < size; ++i) {
        int min = 1e8;
#pragma omp parallel for reduction(min:min), private(j)
        for (j = 1; j < size; ++j) {
            if (A[i][j] < min) {
                min = A[i][j];
            }
        }
        if (min > max) {
            max = min;
        }
    }
    cout << "Max: " << max << endl;

    return max;
}

int maxFromMinsOneThread(Matrix<int> &A) {
    int size = A.getWidth();
    int rowMin[size];
    int i, j;
    int max = 0;
    for (i = 0; i < size; ++i) {
        int min = 1e8;
        for (j = 1; j < size; ++j) {
            if (A[i][j] < min) {
                min = A[i][j];
            }
        }
        if (min > max) {
            max = min;
        }
    }
    cout << "Max: " << max << endl;

    return max;
}

template <typename F>
void printExecTime(F &function, Matrix<int>& A) {
    auto start = chrono::high_resolution_clock::now();
    function(A);
    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>
                            (end - start).count() / 1e9;
    cout << "Elapsed time: " << duration << endl;

}

int main() {
    const int size = 20000;
    Matrix<int> A(size);
    printExecTime(randomizeMatrix, A);
    printExecTime(maxFromMinsOneThread, A);
    printExecTime(maxFromMinsParallel, A);
    return 0;
}
