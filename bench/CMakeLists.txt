
find_package(benchmark REQUIRED)

qv_executable(
  TARGET mutation
  SOURCES benchqv.cc mutation.cc
  PUBLIC_LIBRARIES QV::qv benchmark::benchmark
)
