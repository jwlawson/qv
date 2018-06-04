cmake_minimum_required(VERSION 3.5.2)
project(qv LANGUAGES CXX VERSION 0.17.0)

option(BUILD_SHARED_LIBS
  "Build shared libraries rather than static libraries" ON)
option(QV_BUILD_TESTS "Option to build tests." ON)

if(MSVC)
  set(_warning_flags /W4)
  set(_optimisation_flags "")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(_warning_flags -Wall -Wextra -Werror)
  set(_optimisation_flags
    -march=native -fno-signed-zeros -fno-math-errno -fno-rounding-math
    -fno-signaling-nans -fno-trapping-math -ffinite-math-only
  )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(_warning_flags -Wall -Wextra -Werror)
  set(_optimisation_flags -march=native)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  set(_warning_flags -Wall)
  set(_optimisation_flags -xHOST -ipo -no-prec-div)
else()
  set(_warning_flags "")
  set(_optimisation_flags "")
endif()
set(QV_WARNING_FLAGS "${_warning_flags}"
  CACHE STRING "Warning flags to use when compiling")
list(APPEND CMAKE_CXX_FLAGS_RELEASE "${_optimisation_flags}")
STRING(REPLACE ";" " " CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

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


