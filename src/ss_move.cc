/**
 * ss_move.cc
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
#include "ss_move.h"

namespace cluster {
namespace {
typedef std::vector<SSMove::Applicable> AppVec;
}

AppVec
SSMove::applicable_submatrices(const IntMatrix& m) const {
  return applicable_submatrices(std::make_shared<const IntMatrix>(m));
}
/**
 * Check whether there is a row that only contains zeros, or values with the
 * same sign. That is every arrow at the vertex is pointing the same way.
 */
AppVec
SSMove::applicable_submatrices(CMatrixPtr m) const {
  AppVec result;
  int const* r = m->data();
  int row = 0;
  int col = 0;
  int ind = 0;
  int max = m->num_rows() * m->num_cols();
  int maxcol = m->num_cols();
  int lastcol = maxcol - 1;
  bool pos = false;
  bool neg = false;
  while (ind < max) {
    if (col == 0) {
      pos = false;
      neg = false;
    }
    if (col != row) {
      if (r[ind] > 0) {
        pos = true;
      } else if (r[ind] < 0) {
        neg = true;
      }
    }
    if (col == lastcol) {
      if (pos ^ neg) { /*XOR - one or the other */
        result.emplace_back(row, m);
      }
    }
    ind++;
    col++;
    if (col == maxcol) {
      col = 0;
      row++;
    }
  }
  return result;
}
void
SSMove::move(const Applicable& app, IntMatrix& result) const {
  const int* m = app.matrix_->data();
  int* r = result.data();
  int row = 0;
  int col = 0;
  int maxcol = app.matrix_->num_cols();
  int ind = 0;
  int max = app.matrix_->num_rows() * app.matrix_->num_cols();
  int match = app.row_;
  while (ind < max) {
    if (row == match || col == match) {
      r[ind] = -1 * m[ind];
    } else {
      r[ind] = m[ind];
    }
    ind++;
    col++;
    if (col == maxcol) {
      col = 0;
      row++;
    }
  }
  result.reset();
}
}  // namespace cluster
