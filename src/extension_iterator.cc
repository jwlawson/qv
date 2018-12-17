/*
 * extension_iterator.cc
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
#include "extension_iterator.h"

#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {

template <class T>
ExtensionIterator<T>::ExtensionIterator()
    : matrix_(), size_(0), index_(0), max_(0) {}

template <class T>
ExtensionIterator<T>::ExtensionIterator(const T& matrix)
    : matrix_(matrix.num_rows() + 1, matrix.num_cols() + 1)
    , size_(matrix.num_rows())
    , index_(0)
    , max_(ipow(5, size_)) {
  matrix.enlarge_matrix(matrix_);
}

template <class T>
T const&
ExtensionIterator<T>::next() {
  T& result = matrix_;
  int num(index_);
  int* last_row_ptr = result.data() + size_ * (size_ + 1);
  int* last_col_ptr = result.data() + size_;
  for (int i = 0; i < size_; i++) {
    int val = (((int)(num / ipow(5, i))) % 5) - 2;
    *last_row_ptr = val;
    *last_col_ptr = -val;
    last_row_ptr += 1;
    last_col_ptr += size_ + 1;
  }
  ++index_;
  result.reset();
  return result;
}

template <class T>
bool
ExtensionIterator<T>::has_next() {
  return index_ < max_;
}

template <class T>
long
ExtensionIterator<T>::ipow(int x, int p) {
  long i = 1;
  for (int j = 1; j <= p; j++) i *= x;
  return i;
}

template class ExtensionIterator<QuiverMatrix>;
template class ExtensionIterator<EquivQuiverMatrix>;

}  // namespace cluster
