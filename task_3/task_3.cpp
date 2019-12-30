//
// Created by sergo on 12/27/19.
//
#include <iostream>
#include <chrono>
#include <vector>
#include <omp.h>
#include <random>

#include "matrix.hpp"
#include "task_3_lib.cpp"

using namespace std;

int main() {
    uint64_t height = 5000;
    Matrix<int> M(height);
    randomizeMatrix(M);
    vector<int> V(height, 1);
    randomizeVector(V);
    auto v1 = printExecTime(MatVecMult, M, V);
    auto v2 = printExecTime(MatVecMultRow, M, V);
    auto v3 = printExecTime(MatVecMultCol, M, V);
    auto v4 = printExecTime(MatVecMultCB, M, V);

    cout << (v1 == v2 and v2 == v3 and v3 == v4 ? "COOL" : "NOT COOL") << endl;

    return 0;
}

