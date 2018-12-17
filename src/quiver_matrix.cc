/*
 * quiver_matrix.h
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
 *
 * QuiverMatrix implementation.
 *
 * Most actual implementation is for the template methods and so in the header
 * file.
 */
#include "quiver_matrix.h"

#include <algorithm>

namespace cluster {
QuiverMatrix::QuiverMatrix() : IntMatrix() {}

QuiverMatrix::QuiverMatrix(const int rows, const int cols)
    : IntMatrix(rows, cols) {}

QuiverMatrix::QuiverMatrix(const int rows, const int cols, const int values[])
    : IntMatrix(rows, cols, values) {}

QuiverMatrix::QuiverMatrix(IntMatrix const &matrix) : IntMatrix(matrix) {}

QuiverMatrix::QuiverMatrix(std::string const &str) : IntMatrix(str) {}

bool QuiverMatrix::is_infinite() const {
  return std::any_of(data_.data(), data_.data() + num_rows_ * num_cols_,
                     [](int val) { return val >= 3 || val <= -3; });
}

} // namespace cluster
