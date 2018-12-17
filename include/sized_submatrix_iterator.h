/**
 * sized_submatrix_iterator.h
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
/**
 * Iterator which goes over all square submatrices of a given matrix which are a
 * specified size, attained by removing the same rows and columns.
 *
 * This iterator does not do any checks on the submatrices nor does it ensure
 * that there are no zero rows.
 */
#pragma once

#include "int_matrix.h"

namespace cluster {

class SizedSubmatrixIterator {
public:
  /**
   * Create an iterator which returns submatrices of size `size` of the
   * provided matrix.
   * @param size Size of each submatrix.
   * @param matrix Initial matrix to take submatrices of.
   */
  SizedSubmatrixIterator(int size, const IntMatrix &matrix);
  /**
   * Check if the iterator will return a valid submatrix if next is called.
   * @return true if next() will return a valid submatrix.
   */
  bool has_next();
  /**
   * Get the next submatrix without allocations.
   * The matrix will be stored in the provided matrix. No bounds or size
   * checks are carried out, so ensure that the matrix is the correct size.
   */
  void next(IntMatrix &result);
  /** Get the rows which make up the previously returned submatrix. */
  std::vector<int> get_rows();

private:
  /** Initial matrix to take submatrices of. */
  const IntMatrix &m_;
  /** Size of smallest dimension of m_ */
  int max_val_;
  /** Vector of which rows and columns were in the last returned submatrix. */
  std::vector<int> last_rows_;
  /** Vector of which rows and columns to include in the next submatrix. */
  std::vector<int> rows_;
  /** True if the iterator will return a valid matrix next. */
  bool has_next_;

  /** Compute the next set of rows and columns for the next submatrix. */
  void check_add_index(size_t ind);
};
} // namespace cluster
