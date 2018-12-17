/*
 * int_matrix.h
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
#include "int_matrix.h"

#include <algorithm>
#include <cstring>
#include <functional>
#include <sstream>
/*
 * Cygwin std libc++ is broken and does not contain std::stoi.
 * There are some possible workarounds online involving modifying the lib
 * headers to remove the define guards preventing the function being declared.
 * However I could not get these to work, so instead specify a preprocessor
 * definition in the Makefile if running on Cygwin which enable the following
 * definition of std::stoi using the old fashioned strtol method.
 */
#ifdef CYGWIN_STOI
#include <cstdlib>
namespace std {
int
stoi(const std::string& s) {
  return (int)strtol(s.c_str(), nullptr, 10);
}
}  // namespace std
#endif

namespace cluster {

IntMatrix::IntMatrix() : num_rows_(0), num_cols_(0), data_(0), hashcode_(113) {}

IntMatrix::IntMatrix(const int rows, const int cols)
    : num_rows_(rows)
    , num_cols_(cols)
    , data_(rows * cols, 0)
    , hashcode_(113)
    , recompute_hash_{true} {}

IntMatrix::IntMatrix(const int rows, const int cols, const int values[])
    : num_rows_(rows)
    , num_cols_(cols)
    , data_(values, values + rows * cols)
    , hashcode_(0)
    , recompute_hash_{true} {}

IntMatrix::IntMatrix(std::string const& str)
    : hashcode_{0}, recompute_hash_{true} {
  std::stringstream ss;
  ss << str;
  std::string buf;
  IntVector data;
  data.reserve((str.size() / 2) - 4);
  int row_size = 0;
  int col_size = 0;
  bool start = false;
  bool end = false;
  bool in_row = false;
  bool row_done = false;
  while (!end) {
    ss >> buf;
    if (!start) {
      if ("{" == buf) {
        start = true;
      }
      continue;
    }
    if ("{" == buf) {
      /* Started new row. */
      in_row = true;
    } else if ("}" == buf) {
      /* Reached end of row. */
      if (!in_row) {
        /* End of matrix */
        end = true;
      } else {
        row_done = true;
        in_row = false;
        ++col_size;
      }
    } else {
      /* Should be integer. */
      if (!row_done) {
        ++row_size;
      }
      data.push_back(std::stoi(buf));
    }
  }

  data.shrink_to_fit();

  num_rows_ = row_size;
  num_cols_ = col_size;
  data_ = std::move(data);
}

/* Public methods */
const std::vector<int>
IntMatrix::get_row(const int row) const {
  std::vector<int> result(num_cols_);
  get_row(row, result);
  return result;
}

void
IntMatrix::get_row(const int row, std::vector<int>& result) const {
  int count = row * num_cols_;
  std::memcpy(result.data(), data_.data() + count, num_cols_ * sizeof(int));
}

const std::vector<int>
IntMatrix::get_col(const int col) const {
  std::vector<int> result(num_rows_);
  get_col(col, result);
  return result;
}

void
IntMatrix::get_col(const int col, std::vector<int>& result) const {
  int count = col;
  for (int j = 0; j < num_rows_; j++) {
    result[j] = data_[count];
    count += num_cols_;
  }
}

int
IntMatrix::zero_row() const {
  bool isZero = false;
  int row = -1;
  for (int ind = 0; ind < num_rows_ * num_cols_; ind++) {
    if (ind % num_cols_ == 0) {
      if (isZero) {
        return row;
      }
      row++;
      isZero = true;
    }
    if (data_[ind] != 0) {
      isZero = false;
    }
  }
  if (isZero) {
    return row;
  }
  return -1;
}

/*
 * Cannot check hashcode here, as implementing classes can change the way the
 * hashcode is computed. This means that two matrices with the same entries
 * can have different hashcodes if they are instances of different classes.
 */
bool
IntMatrix::are_equal(const IntMatrix& lhs, const IntMatrix& rhs) {
  return lhs.data_ == rhs.data_;
}

void
IntMatrix::submatrix(const int row, const int col, IntMatrix& result) const {
  int resInd = 0;
  int origInd = 0;

  while (resInd < result.num_rows_ * result.num_cols_) {
    bool changed;
    do {
      changed = false;
      if (origInd == row * num_cols_) {
        origInd += num_cols_;
        changed = true;
      }
      if (origInd % num_cols_ == col) {
        origInd++;
        changed = true;
      }
    } while (changed);
    result.data_[resInd++] = data_[origInd++];
  }
  result.reset();
}

void
IntMatrix::submatrix(std::vector<int> rows, std::vector<int> cols,
                     IntMatrix& result) const {
  using std::size_t;
  int row_ind = 0;
  int col_ind = 0;
  size_t rows_vec_ind = 0;
  size_t cols_vec_ind = 0;
  int this_index = 0;
  int result_index = 0;
  /* Sort the vectors and keep track of the current index to avoid searching
   * through the arrays continuously. */
  std::sort(rows.begin(), rows.end());
  std::sort(cols.begin(), cols.end());

  while (row_ind < num_rows_) {
    if (rows_vec_ind < rows.size() && rows[rows_vec_ind] == row_ind) {
      rows_vec_ind++;
      col_ind = 0;
      cols_vec_ind = 0;
      while (col_ind < num_cols_) {
        if (cols_vec_ind < cols.size() && cols[cols_vec_ind] == col_ind) {
          cols_vec_ind++;
          result.data_[result_index++] = data_[this_index++];
        } else {
          this_index++;
        }
        col_ind++;
      }
    } else {
      this_index += num_cols_;
    }
    row_ind++;
  }
  result.reset();
}

void
IntMatrix::enlarge_matrix(IntMatrix& result) const {
  int sub = 0;
  for (int index = 0; index < result.num_rows_ * result.num_cols_; index++) {
    if (index % result.num_cols_ >= num_cols_) {
      result.data_[index] = 0;
      sub++;
    } else if (index >= num_rows_ * result.num_cols_) {
      result.data_[index] = 0;
    } else {
      result.data_[index] = data_[index - sub];
    }
  }
  result.reset();
}

void
IntMatrix::permute_rows(const std::vector<int>& vec, IntMatrix& result) const {
  int ind = 0;
  for (int i = 0; i < num_rows_; ++i) {
    int offset = vec[i] * num_cols_;
    std::memcpy(result.data_.data() + offset, data_.data() + ind,
                num_cols_ * sizeof(int));
    ind += num_cols_;
  }
  result.reset();
}

void
IntMatrix::permute_columns(const std::vector<int>& vec,
                           IntMatrix& result) const {
  int ind = -1;
  for (int i = 0; i < num_rows_; ++i) {
    int offset = i * num_cols_;
    for (int j = 0; j < num_cols_; ++j) {
      result.data_[offset + vec[j]] = data_[++ind];
    }
  }
  result.reset();
}

/* Private methods */

std::size_t
IntMatrix::compute_hash() const {
  std::size_t hash = 113;
  int_fast16_t max = num_rows_ * num_cols_;
  for (int_fast16_t i = 0; i < max; i++) {
    hash *= 31;
    hash += data_[i];
  }
  return hash;
}

void
IntMatrix::mult(const IntMatrix& left, const IntMatrix& right,
                IntMatrix& result) const {
  int col_inc = right.num_rows_;
  int leftInd;
  int leftIndStart = 0;
  int rightInd;
  int calcInd = 0;
  for (int i = 0; i < left.num_rows_; i++) {
    for (int j = 0; j < right.num_cols_; j++) {
      leftInd = leftIndStart;
      rightInd = j;
      result.data_[calcInd] = 0;
      while (leftInd < leftIndStart + left.num_cols_) {
        result.data_[calcInd] += left.data_[leftInd] * right.data_[rightInd];
        leftInd++;
        rightInd += col_inc;
      }
      calcInd++;
    }
    leftIndStart += left.num_cols_;
  }
  result.reset();
}

/* Friends */

std::ostream&
operator<<(std::ostream& os, const IntMatrix& mat) {
  os << "{ ";
  for (int i = 0; i < mat.num_rows_; i++) {
    os << "{ ";
    for (int j = 0; j < mat.num_cols_; j++) {
      os << mat.data_[i * mat.num_cols_ + j] << " ";
    }
    os << "} ";
  }
  os << "}";
  return os;
}

void
swap(IntMatrix& first, IntMatrix& second) {
  using std::swap;
  swap(first.data_, second.data_);
  swap(first.num_rows_, second.num_rows_);
  swap(first.num_cols_, second.num_cols_);
  swap(first.hashcode_, second.hashcode_);
  swap(first.recompute_hash_, second.recompute_hash_);
}

}  // namespace cluster
