/*
 * array_utils.cc
 * Copyright 2014-2015 John Lawson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "array_utils.h"
#include <algorithm>

#include <boost/functional/hash.hpp>

namespace cluster {
namespace arrays {

int*
sort(int* arr, const int size) {
  std::sort(arr, arr + size);
  return arr;
}

std::size_t
hash(const int* arr, const int size) {
  return boost::hash_range(arr, arr + size);
}

std::size_t
hash(const std::vector<std::pair<int, int>>& vec) {
  size_t hash{47};
  for (size_t i = 0; i < vec.size(); ++i) {
    hash *= 31;
    hash += vec[i].first;
  }
  for (size_t i = 0; i < vec.size(); ++i) {
    hash *= 31;
    hash += vec[i].second;
  }
  return hash;
}

int
number_in(const std::vector<int>& arr, const int val) {
  int count = 0;
  for (size_t i = 0; i < arr.size(); i++) {
    if (arr[i] == val) {
      count++;
    }
  }
  return count;
}

bool
equal(const std::vector<int>& a, const std::vector<int>& b) {
  if (a.size() != b.size()) {
    return false;
  }
  for (size_t i = 0; i < a.size(); i++) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

}  // namespace arrays
}  // namespace cluster
