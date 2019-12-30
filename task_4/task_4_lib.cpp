#ifndef __TASK_4_LIB__
#define __TASK_4_LIB__

#include <omp.h>
#include <chrono>
#include <iostream>

#include "matrix.hpp"


using namespace std;


template <typename F>
Matrix<uint64_t>* printExecTime(F &function, Matrix<uint64_t>& A, Matrix<uint64_t>& B) {
    auto start = chrono::high_resolution_clock::now();
    auto res = function(A, B);
    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>
                            (end - start).count() / 1e9;
    cout << "Elapsed time: " << duration << endl;
    return res;
}

Matrix<uint64_t>* prodOneThread(Matrix<uint64_t>& A, Matrix<uint64_t>& B) {
    if (A.getWidth() != B.getHeight()) {
        throw runtime_error("Wrong matrices size");
    }
    auto result = new Matrix<uint64_t>(A.getHeight(), B.getWidth());

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

// block-striped data decomposition
Matrix<uint64_t>* prodBS(Matrix<uint64_t> &A, Matrix<uint64_t> &B) {
    if (A.getWidth() != B.getHeight()) {
        throw runtime_error("Wrong size");
    }
    auto resP = new Matrix<uint64_t>(A.getHeight(), B.getWidth());
    auto &res = *resP;

#pragma omp parallel shared(A, B, res, cout) default(none)
    {
        int threadsNum = omp_get_num_threads();
#pragma omp for schedule(static) collapse(2)
        for (int iVer = 0; iVer < threadsNum; ++iVer) {
            for (int iHor = 0; iHor < threadsNum; ++iHor) {
                int ABlockBegin = iVer * A.getHeight() / threadsNum;
                int ABlockEnd = (iVer + 1) * A.getHeight() / threadsNum;
                int BBlockBegin = iHor * B.getWidth() / threadsNum;
                int BBlockEnd = (iHor + 1) * B.getWidth() / threadsNum;
                // cout << threadsNum << ";" << thread << endl
                //     << ABlockBegin << ":" << ABlockEnd << ":" << BBlockBegin << ":" << BBlockEnd << endl;
                for (int i = ABlockBegin; i < ABlockEnd; ++i) {
                    for (int j = BBlockBegin; j < BBlockEnd; ++j) {
                        res[i][j] = A[i][0] * B[0][j];
                        for (int k = 1; k < A.getWidth(); ++k) {
                            res[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
    

    return resP;
}

// checkerboard data decomposition
Matrix<uint64_t>* prodCB(Matrix<uint64_t> &A, Matrix<uint64_t> &B) {
    if (A.getWidth() != B.getHeight()) {
        throw runtime_error("Wrong size");
    }
    auto resP = new Matrix<uint64_t>(A.getHeight(), B.getWidth());
    auto &res = *resP;

#pragma omp parallel shared(A, B, res) default(none)
    {
        int blocksVer = omp_get_num_threads();
        int blocksHor = omp_get_num_threads();
        int blocksK = omp_get_num_threads();
#pragma omp for
        for (int i = 0; i < res.getHeight(); ++i) {
            for (int j = 0; j < res.getWidth(); ++j) {
                res[i][j] = 0;
            }
        }

#pragma omp for collapse(2)
        for (int vi = 0; vi < blocksVer; ++vi) {
            for (int hi = 0; hi < blocksHor; ++hi) {
                for (int ki = 0; ki < blocksK; ++ki) {
                    int vbb = vi * A.getHeight() / blocksVer; // vertical block begin
                    int vbe = (vi + 1) * A.getHeight() / blocksVer; // vertical block end
                    int hbb = hi * B.getWidth() / blocksHor; // vertical block begin
                    int hbe = (hi + 1) * B.getWidth() / blocksHor; // vertical block end
                    int kbb = ki * A.getWidth() / blocksK; // k block begin
                    int kbe = (ki + 1) * A.getWidth() / blocksK; // k block begin

                    for (int i = vbb; i < vbe; ++i) {
                        for (int j = hbb; j < hbe; ++j) {
                            for (int k = kbb; k < kbe; ++k) {
#pragma omp atomic
                                res[i][j] += A[i][k] * B[k][j];
                            }
                        }
                    }
                }
            }
        }
    }

    return resP;
}

#endif