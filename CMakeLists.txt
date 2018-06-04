cmake_minimum_required(VERSION 3.5.2)
project(qv LANGUAGES CXX VERSION 0.17.0)

option(QV_BUILD_TESTS "Option to build tests." ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
include(QVHelpers)

find_package(Boost REQUIRED)
find_package(GiNaC REQUIRED)

add_subdirectory(src)

if(QV_BUILD_TESTS)
  find_package(GTest REQUIRED)
  enable_testing()
  add_subdirectory(test)
endif()


