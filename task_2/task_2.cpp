//
// Created by sergo on 12/6/19.
//
#include <iostream>
#include <chrono>
#include "matrix.hpp"

using namespace std;

template <typename F>
Matrix<int>* printExecTime(F &function, Matrix<int>& A, Matrix<int>& B) {
    auto start = chrono::high_resolution_clock::now();
    auto res = function(A, B);
    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>
                            (end - start).count() / 1e9;
    cout << "Elapsed time: " << duration << endl;
    return res;
}

Matrix<int>* prodOneThread(Matrix<int>& A, Matrix<int>& B) {
    if (A.getWidth() != B.getWidth()) {
        throw runtime_error("Wrong matrices size");
    }
    auto result = new Matrix<int>(A.getHeight(), B.getWidth());

    auto &C = *result;
    for (int i = 0; i < A.getHeight(); ++i) {
        for (int j = 0; j < B.getWidth(); ++j) {
            C[i][j] = 0;
            for (int k = 0; k < B.getHeight(); ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

Matrix<int>* prodParallel(Matrix<int>& A, Matrix<int>& B) {
    if (A.getWidth() != B.getWidth()) {
        throw runtime_error("Wrong matrices size");
    }
    auto result = new Matrix<int>(A.getHeight(), B.getWidth());

    auto &C = *result;
    int i, j, k;
#pragma parallel for, private(i)
    for (i = 0; i < A.getHeight(); ++i) {
#pragma parallel for, private(i, j)
        for (j = 0; j < B.getWidth(); ++j) {
//            C[i][j] = 0;
            int sum = 0;
#pragma parallel for reduction(+:sum), private(i, j, k)
            for (k = 0; k < B.getHeight(); ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    return result;
}

int main() {
    int size = 1000;
    Matrix<int> A(size), B(size);

    randomizeMatrix(A);
    randomizeMatrix(B);

    auto C = printExecTime(prodOneThread, A, B);
    auto D = printExecTime(prodParallel, A, B);

    if (*C == *D) {
        cout << "COOL" << endl;
    }
    else {
        cout << "NOT COOL" << endl;
    }

    return 0;
}

