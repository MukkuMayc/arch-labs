#include <benchmark/benchmark.h>

#include "matrix.hpp"
#include "task_4_lib.cpp"


static void Block_striped_decomposition(benchmark::State &state) {
  int size = 800;
  for (auto _: state) {
    state.PauseTiming();
    Matrix<uint64_t> A(size);
    randomizeMatrix(A);
    Matrix<uint64_t> B(size);
    randomizeMatrix(B);
    state.ResumeTiming();

    prodBS(A, B, state.range(0));
  }
}

static void Checkerboard_decomposition(benchmark::State &state) {
  int size = 800;
  for (auto _: state) {
    state.PauseTiming();
    Matrix<uint64_t> A(size);
    randomizeMatrix(A);
    Matrix<uint64_t> B(size);
    randomizeMatrix(B);
    state.ResumeTiming();

    prodCB(A, B, state.range(0));
  }
}

BENCHMARK(Block_striped_decomposition)
    ->DenseRange(1, 8)
    ->Iterations(5);

BENCHMARK(Checkerboard_decomposition)
    ->DenseRange(1, 8)
    ->Iterations(5);

BENCHMARK_MAIN();