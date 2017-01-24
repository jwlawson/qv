#include <benchmark/benchmark.h>

#include "dynkin.h"
#include "quiver_matrix.h"

static void quiver_mutate_markov(benchmark::State& state) {
  cluster::QuiverMatrix q{"{ { 0 2 -2 } { -2 0 2 } { 2 -2 0 } }"};
  cluster::QuiverMatrix r(3,3);
  const int v = state.range(0);
  while(state.KeepRunning()) {
    q.mutate(v, r);
    benchmark::DoNotOptimize(r);
  }
}
static void quiver_mutate_a9(benchmark::State& state) {
  cluster::QuiverMatrix q = cluster::dynkin::A9;
  cluster::QuiverMatrix r(9,9);
  const int v = state.range(0);
  while(state.KeepRunning()) {
    q.mutate(v, r);
    benchmark::DoNotOptimize(r);
  }
}
static void quiver_mutate_Aa6(benchmark::State& state) {
  cluster::QuiverMatrix q {"{ { 0 1 0 0 0 -1 } "
                             "{ -1 0 1 0 0 0 } "
                             "{ 0 -1 0 1 0 0 } "
                             "{ 0 0 -1 0 1 0 } "
                             "{ 0 0 0 -1 0 1 } "
                             "{ 1 0 0 0 -1 0 } }"};
  cluster::QuiverMatrix r(6,6);
  const int v = state.range(0);
  while(state.KeepRunning()) {
    q.mutate(v, r);
    benchmark::DoNotOptimize(r);
  }
}
static void quiver_mutate_dense(benchmark::State& state) {
  int iv[] { 0, 1, 1, 1, 1, 1,
            -1, 0, 1, 1, 1, 1,
            -1,-1, 0, 1, 1, 1,
            -1,-1,-1, 0, 1, 1,
            -1,-1,-1,-1, 0, 1,
            -1,-1,-1,-1,-1, 0
  };
  cluster::QuiverMatrix q{6, 6, iv};
  cluster::QuiverMatrix r(6, 6);
  int v = state.range(0);
  while(state.KeepRunning()) {
    q.mutate(v, r);
    benchmark::DoNotOptimize(r);
  }
}

BENCHMARK(quiver_mutate_markov)->Range(0, 2);
BENCHMARK(quiver_mutate_a9)->RangeMultiplier(2)->Range(0, 8);
BENCHMARK(quiver_mutate_Aa6)->RangeMultiplier(2)->Range(0, 5);
BENCHMARK(quiver_mutate_dense)->RangeMultiplier(2)->Range(0, 5);

