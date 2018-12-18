
find_package(benchmark REQUIRED)

add_library(bench_main STATIC benchqv.cc)

qv_executable(
  TARGET mutation
  SOURCES mutation.cc
  PUBLIC_LIBRARIES QV::qv benchmark::benchmark
  PRIVATE_LIBRARIES bench_main
)
qv_executable(
  TARGET mutation_class
  SOURCES mutation_class.cc
  PUBLIC_LIBRARIES QV::qv benchmark::benchmark
  PRIVATE_LIBRARIES bench_main
)
qv_executable(
  TARGET fast_infinite
  SOURCES fast_inf.cc
  PUBLIC_LIBRARIES QV::qv benchmark::benchmark
  PRIVATE_LIBRARIES bench_main
)
