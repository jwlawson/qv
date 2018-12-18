#include <benchmark/benchmark.h>

#include "dynkin.h"
#include "mutation_class.h"

static void
quiver_mutation_class_a3(benchmark::State& state) {
  cluster::QuiverMatrix q = cluster::dynkin::A3;
  for (auto _ : state) {
    auto x = cluster::MutationClass(q);
    benchmark::DoNotOptimize(x);
  }
}
BENCHMARK(quiver_mutation_class_a3);

static void
quiver_mutation_class_a4(benchmark::State& state) {
  cluster::QuiverMatrix q = cluster::dynkin::A4;
  for (auto _ : state) {
    auto x = cluster::MutationClass(q);
    benchmark::DoNotOptimize(x);
  }
}
BENCHMARK(quiver_mutation_class_a4);

static void
quiver_mutation_class_a5(benchmark::State& state) {
  cluster::QuiverMatrix q = cluster::dynkin::A5;
  for (auto _ : state) {
    auto x = cluster::MutationClass(q);
    benchmark::DoNotOptimize(x);
  }
}
BENCHMARK(quiver_mutation_class_a5);

