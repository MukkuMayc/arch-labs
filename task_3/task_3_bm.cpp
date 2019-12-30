#include <benchmark/benchmark.h>

#include "matrix.hpp"
#include "task_3_lib.cpp"


static void Rowwise_decomposition(benchmark::State &state) {
  int size = 5000;
  for (auto _: state) {
    state.PauseTiming();
    Matrix<int>* M = new Matrix<int>(size);
    randomizeMatrix(*M);
    vector<int> V(size);
    randomizeVector(V);
    state.ResumeTiming();

    MatVecMultRow(*M, V, state.range(0));
    
    state.PauseTiming();
    delete M;
    state.ResumeTiming();
  }
}

static void Columnwise_decomposition(benchmark::State &state) {
  int size = 5000;
  for (auto _: state) {
    state.PauseTiming();
    Matrix<int> M(size);
    randomizeMatrix(M);
    vector<int> V(size);
    randomizeVector(V);
    state.ResumeTiming();

    MatVecMultCol(M, V, state.range(0));
  }
}

static void Checkerboard_decomposition(benchmark::State &state) {
  int size = 5000;
  for (auto _: state) {
    state.PauseTiming();
    Matrix<int> M(size);
    randomizeMatrix(M);
    vector<int> V(size);
    randomizeVector(V);
    state.ResumeTiming();

    MatVecMultCB(M, V, state.range(0));
  }
}

BENCHMARK(Rowwise_decomposition)
    ->DenseRange(1, 8)
    ->Iterations(10);

BENCHMARK(Columnwise_decomposition)
    ->DenseRange(1, 8)
    ->Iterations(10);

BENCHMARK(Checkerboard_decomposition)
    ->DenseRange(1, 8)
    ->Iterations(10);

BENCHMARK_MAIN();