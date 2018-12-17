/*
 * skew_symmetric_iterator.cc
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
#include "skew_symmetric_iterator.h"

namespace cluster {

SkewSymmetricIterator::SkewSymmetricIterator(const int size)
    : matrix_(size, size), col_comp_(size - 1), index_(),
      num_vars_(num_vars(size)), max_(ipow(5, num_vars_)) {}

bool SkewSymmetricIterator::has_next() const { return index_ < max_; }

// TODO Only set the entries which need to be changed
SkewSymmetricIterator::MPtr SkewSymmetricIterator::next() {
  int num(index_);
  int row = -1;
  int col = 0;
  for (int i = 0; i < num_vars_; ++i) {
    if (col % col_comp_ == 0) {
      ++row;
      col = row + 1;
    } else {
      ++col;
    }
    int val = (((int)(num / ipow(5, i))) % 5) - 2;
    matrix_.set(row, col, val);
    matrix_.set(col, row, -val);
  }
  ++index_;
  if (matrix_.zero_row() != -1) {
    /*
     * This is fine, as the very last matrix will have 2 in all possible
     * positions, hence has no zero rows, so there is no chance of returning a
     * broken value.
     */
    return next();
  }
  return std::make_shared<M>(matrix_);
}

long int SkewSymmetricIterator::ipow(const int x, const int p) const {
  long i = 1;
  for (int j = 1; j <= p; j++)
    i *= x;
  return i;
}

int SkewSymmetricIterator::num_vars(int size) const {
  return ((size - 1) * size) / 2;
}
} // namespace cluster
