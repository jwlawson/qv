/*
 * mmi.cc
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
#include "mmi.h"

#include "equiv_mutation_class_size.h"
#include "equiv_quiver_matrix.h"
#include "fast_infinite.h"
#include "mass_finite_check.h"
#include "submatrix_iterator.h"

namespace cluster {
namespace mmi {

namespace {
MassFiniteCheck chk;
} // namespace

void add_finite(const std::shared_ptr<EquivQuiverMatrix> &mat) {
  chk.add_finite(mat);
}

bool fast_mmi(const QuiverMatrix &matrix) {
  if (!fastinf::is_infinite(matrix)) {
    EquivQuiverMatrix e(matrix);
    if (chk.is_finite(e)) {
      /* Actually finite. */
      return false;
    }
  }
  SubmatrixIterator<QuiverMatrix> iter(matrix);
  while (iter.has_next()) {
    QuiverMatrix const &n = iter.next();
    if (fastinf::is_infinite(n)) {
      return false;
    }
  }
  return true;
}

bool slow_mmi(const QuiverMatrix &matrix) {
  if (!fastinf::is_infinite(matrix)) {
    /* Matrix probably finite. */
    EquivQuiverMatrix e(matrix);
    if (chk.is_finite(e)) {
      /* Actually finite. */
      return false;
    }
  }
  /* Now check submatrices. */
  SubmatrixIterator<QuiverMatrix> iter(matrix);
  while (iter.has_next()) {
    QuiverMatrix const &n = iter.next();
    if (fastinf::is_infinite(n)) {
      return false;
    }
    /* Probably finite. */
    EquivQuiverMatrix e(n);
    if (!chk.is_finite(e)) {
      /* Actually infinite. */
      return false;
    }
  }
  return true;
}

} // namespace mmi
} // namespace cluster
