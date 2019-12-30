#ifndef __TASK_3_LIB__
#define __TASK_3_LIB__

#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <omp.h>
#include "matrix.hpp"

using namespace std;

template <typename F>
vector<int> printExecTime(F &function, Matrix<int>& M, vector<int> &V) {
    auto start = chrono::high_resolution_clock::now();
    vector<int> res = function(M, V);
    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>
                            (end - start).count() / 1e9;
    cout << "Elapsed time: " << duration << endl;
    return res;
}

vector<int> MatVecMult(Matrix<int> M, vector<int> V) {
    if (M.getWidth() != V.size()) {
        throw runtime_error("Wrong size");
    }
    vector<int> res(M.getHeight());
    for (int i = 0; i < M.getHeight(); ++i) {
        res[i] = 0;
        for (int j = 0; j < M.getWidth(); ++j) {
            res[i] += M[i][j] * V[i];
        }
    }

    return res;
}

// rowwise data decomposition
vector<int> MatVecMultRow(Matrix<int> &M, vector<int> &V, int threads = 4) {
    if (M.getWidth() != V.size()) {
        throw runtime_error("Wrong size");
    }

    vector<int> res(M.getHeight());
#pragma omp parallel for shared(M, V, res) default(none) num_threads(threads)
    for (int i = 0; i < M.getHeight(); ++i) {
        res[i] = 0;
        for (int j = 0; j < M.getWidth(); ++j) {
            res[i] += M[i][j] * V[i];
        }
    }

    return res;
}

// columnwise data decomposition
vector<int> MatVecMultCol(Matrix<int> &M, vector<int> &V, int threads = 4) {
    if (M.getWidth() != V.size()) {
        throw runtime_error("Wrong size");
    }

    vector<int> res(M.getHeight());
#pragma omp parallel  shared(M, V, res) default(none) num_threads(threads)
    {
#pragma omp for
        for (int i = 0; i < res.size(); ++i) {
            res[i] = 0;
        }

#pragma omp for
        for (int j = 0; j < M.getWidth(); ++j) {
            for (int i = 0; i < M.getHeight(); ++i) {
#pragma omp atomic
                res[i] += M[i][j] * V[i];
            }
        }
    };

    return res;
}

// checkerboard data decomposition
vector<int> MatVecMultCB(Matrix<int> &M, vector<int> &V, int threads = 4) {
    if (M.getWidth() != V.size()) {
        throw runtime_error("Wrong size");
    }

    int blocksVer = omp_get_num_threads();
    int blocksHor = omp_get_num_threads();

    vector<int> res(M.getHeight());

#pragma omp parallel shared(M, V, res, blocksVer, blocksHor) default(none) num_threads(threads)
    {
#pragma omp for
        for (int i = 0; i < res.size(); ++i) {
            res[i] = 0;
        }

#pragma omp for collapse(2)
        for (int vi = 0; vi < blocksVer; ++vi) {
            for (int hi = 0; hi < blocksHor; ++hi) {
                int vbb = vi / blocksVer * M.getHeight(); // vertical block begin
                int vbe = (vi + 1) / blocksVer * M.getHeight(); // vertical block end
                int hbb = hi / blocksHor * M.getWidth(); // vertical block begin
                int hbe = (hi + 1) / blocksHor * M.getWidth(); // vertical block end
                for (int i = vbb; i < vbe; ++i) {
                    for (int j = hbb; j < hbe; ++j) {
#pragma omp atomic
                        res[i] += M[i][j] * V[i];
                    }
                }
            }
        }
    }

    return res;
}


void randomizeVector(vector<int> &V) {
    int low = 0;
    int high = 1e6;
    auto now = chrono::system_clock::now();
    default_random_engine generator(now.time_since_epoch().count());
    uniform_int_distribution<int> distribution(low, high);
    for (int i = 0; i < V.size(); ++i) {
        V[i] = distribution(generator);
    }
}

#endif