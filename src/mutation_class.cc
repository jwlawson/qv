/*
 * mutation_class.cc
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
#include "mutation_class.h"

#include "equiv_mutation_class_loader.h"
#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {

MutationClass::MutationClass(const IntMatrix &initial) : set_(), finite_(true) {
  EquivQuiverMatrix mat(initial);
  EquivMutationClassLoader loader(mat);
  while (loader.has_next()) {
    MPtr p = loader.next_ptr();
    if ((*p).equals(EquivMutationClassLoader::INFINITE)) {
      finite_ = false;
      break;
    }
    set_.insert(p);
  }
}

/*
 * Don't need to create a new EquivQuiverMatrix if that is what is supplied.
 */
MutationClass::MutationClass(const EquivQuiverMatrix &initial)
    : set_(), finite_(true) {
  EquivMutationClassLoader loader(initial);
  while (loader.has_next()) {
    MPtr p = loader.next_ptr();
    if ((*p).equals(EquivMutationClassLoader::INFINITE)) {
      finite_ = false;
      break;
    }
    set_.insert(p);
  }
}

bool MutationClass::contains(std::shared_ptr<M> matrix) const {
  return set_.find(matrix) != set_.end();
}

bool MutationClass::is_finite() const { return finite_; }

std::size_t MutationClass::size() const { return set_.size(); }

typename MutationClass::Iter MutationClass::begin() const {
  return set_.begin();
}

typename MutationClass::Iter MutationClass::end() const { return set_.end(); }

} // namespace cluster
