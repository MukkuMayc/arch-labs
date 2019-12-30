//
// Created by sergo on 12/27/19.
//
#include <iostream>
#include <chrono>
#include <vector>
#include <omp.h>
#include <random>

#include "matrix.hpp"
#include "task_4_lib.cpp"


using namespace std;


int main() {
    uint64_t size = 1000;
    Matrix<uint64_t> A(size);
    randomizeMatrix(A);
    Matrix<uint64_t> B(size);
    randomizeMatrix(B);
    auto res1 = printExecTime(prodOneThread, A, B);
    auto res2 = printExecTime(prodBS, A, B);
    auto res3 = printExecTime(prodCB, A, B);

    cout << (*res1 == *res2 and *res2 == *res3 ? "COOL" : "NOT COOL") << endl;

    return 0;
}

