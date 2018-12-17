/*
 * submatrix_iterator.h
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
 * Compute all submatrices of a given matrix.
 */
#pragma once

#include <memory>

namespace cluster {
template <class T>
class SubmatrixIterator {
 public:
  SubmatrixIterator();
  SubmatrixIterator(const T& initial);

  T const& next();
  bool has_next();

 private:
  T matrix_;
  T submatrix_;
  int count_;
  int max_;
};
}  // namespace cluster
