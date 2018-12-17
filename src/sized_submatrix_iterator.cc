/**
 * sized_submatrix_iterator.cc
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
#include "sized_submatrix_iterator.h"

namespace cluster {
SizedSubmatrixIterator::SizedSubmatrixIterator(int size,
                                               const IntMatrix &matrix)
    : m_(matrix), max_val_(std::min(matrix.num_rows(), matrix.num_cols()) - 1),
      last_rows_(size), rows_(size), has_next_(true) {
  for (size_t i = 0; i < rows_.size(); i++) {
    rows_[i] = i;
  }
}
bool SizedSubmatrixIterator::has_next() { return has_next_; }
void SizedSubmatrixIterator::next(IntMatrix &result) {
  m_.submatrix(rows_, rows_, result);
  last_rows_.assign(rows_.begin(), rows_.end());
  check_add_index(0);
}
std::vector<int> SizedSubmatrixIterator::get_rows() { return last_rows_; }
void SizedSubmatrixIterator::check_add_index(size_t ind) {
  if (rows_[ind] == max_val_) {
    has_next_ = false;
    return;
  }
  if (ind != rows_.size() - 1 && rows_[ind] + 1 == rows_[ind + 1]) {
    rows_[ind] = ind;
    return check_add_index(ind + 1);
  } else {
    rows_[ind] += 1;
  }
}
} // namespace cluster
