cmake_minimum_required(VERSION 3.5.1)
project(qv LANGUAGES CXX VERSION 0.17.0)

option(BUILD_SHARED_LIBS
  "Build shared libraries rather than static libraries" ON)
option(QV_BUILD_TESTS "Enable building tests." ON)
if(QV_BUILD_TESTS)
  option(QV_TEST_GINAC "Enable tests which GiNaC library" ON)
endif()
option(QV_BUILD_BENCH "Enable building benchmarks" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
include(QVHelpers)

find_package(Boost REQUIRED)
if(Boost_FOUND AND NOT TARGET Boost::boost)
  add_library(Boost::boost INTERFACE IMPORTED)
  set_target_properties(Boost::boost PROPERTIES
    INTERFACE_INCLUDE_DIRS ${Boost_INCLUDE_DIRS}
  )
endif()
add_subdirectory(src)

if(QV_BUILD_TESTS)
  find_package(GTest REQUIRED)
  if(QV_TEST_GINAC)
    find_package(GiNaC REQUIRED)
  endif()

  enable_testing()
  add_subdirectory(test)
endif()

if(QV_BUILD_BENCH)
  add_subdirectory(bench)
endif()
