/*
 * class_ext_iterator.h
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
 * Contains EquivMutClassExtIterator - a class to iterate over the whole
 * mutation class of the seed matrix and return every possible skew-symetric
 * extension of each member of the class.
 */
#pragma once

#include <memory>

#include "equiv_mutation_class_loader.h"
#include "extension_iterator.h"
#include "quiver_matrix.h"

namespace cluster {

class EquivMutClassExtIterator {
public:
  /**
   * Default constructor. Does nothing much.
   */
  EquivMutClassExtIterator();

  /**
   * Create an iterator with the specified seed matrix. All extensions of
   * all members in the matrix mutation class will be returned by the
   * iterator.
   * @param matrix Matrix to seed to the mutation class
   */
  EquivMutClassExtIterator(QuiverMatrix const &matrix);

  /**
   * Get the next matrix from the iterator.
   */
  QuiverMatrix const &next();

  /**
   * Check whether the iterator has another matrix to return.
   * @return true if next will return a valid matrix
   */
  bool has_next();

private:
  EquivMutationClassLoader class_;
  ExtensionIterator<QuiverMatrix> iter_;
};

} // namespace cluster
