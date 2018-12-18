#include <benchmark/benchmark.h>

#include "dynkin.h"
#include "fast_infinite.h"

static void
fast_infinite_a3(benchmark::State& state) {
  cluster::QuiverMatrix q = cluster::dynkin::A3;
  for (auto _ : state) {
    benchmark::DoNotOptimize(cluster::fastinf::is_infinite(q));
  }
}
BENCHMARK(fast_infinite_a3);

static void
fast_infinite_markov(benchmark::State& state) {
  cluster::QuiverMatrix q{"{ { 0 2 -2 } { -2 0 2 } { 2 -2 0 } }"};
  for (auto _ : state) {
    benchmark::DoNotOptimize(cluster::fastinf::is_infinite(q));
  }
}
BENCHMARK(fast_infinite_markov);


static void
fast_infinite_aa6(benchmark::State& state) {
  cluster::QuiverMatrix q{
    "{ { 0 1 0 0 0 -1 } "
      "{ -1 0 1 0 0 0 } "
      "{ 0 -1 0 1 0 0 } "
      "{ 0 0 -1 0 1 0 } "
      "{ 0 0 0 -1 0 1 } "
      "{ 1 0 0 0 -1 0 } }"};
  for (auto _ : state) {
    benchmark::DoNotOptimize(cluster::fastinf::is_infinite(q));
  }
}
BENCHMARK(fast_infinite_aa6);
