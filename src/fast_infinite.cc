/*
 * fast_infinite.cc
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
#include "fast_infinite.h"
#include <random>
#include <vector>

namespace cluster {
namespace fastinf {

namespace {
std::default_random_engine gen;
std::vector<QuiverMatrix> mutated(2, QuiverMatrix());
std::uniform_int_distribution<int> dist;
}  // namespace

bool
is_infinite(const QuiverMatrix& matrix) {
  // All 2x2 matrices are mutation finite
  if (matrix.num_rows() <= 2 && matrix.num_cols() <= 2) {
    return false;
  }
  mutated[0].set_matrix(matrix);
  if (mutated[1].num_rows() != matrix.num_rows()) {
    mutated[1].set_matrix(matrix);
  }
  int lastMutation = -1;
  int last_count;
  int counter = 0;
  if (dist.max() != matrix.num_rows() - 1 || dist.min() != 0) {
    dist = std::uniform_int_distribution<int>(0, matrix.num_rows() - 1);
  }
  auto random = []() { return dist(gen); };

  while (counter < MAX_MUTATIONS) {
    int rand;
    do {
      rand = random();
    } while (rand == lastMutation);

    /* Alternate between mutating the two matrices in the array. */
    last_count = counter % 2;
    counter++;
    mutated[last_count].mutate(rand, mutated[counter % 2]);
    if (mutated[counter % 2].is_infinite()) {
      return true;
    }
    lastMutation = rand;
  }
  return false;
}

}  // namespace fastinf
}  // namespace cluster
