/*
 * class_ext_iterator.cc
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
#include "class_ext_iterator.h"

namespace cluster {

EquivMutClassExtIterator::EquivMutClassExtIterator() : class_(), iter_() {}

EquivMutClassExtIterator::EquivMutClassExtIterator(QuiverMatrix const& mat)
    : class_(mat), iter_() {}

QuiverMatrix const&
EquivMutClassExtIterator::next() {
  if (!iter_.has_next()) {
    iter_ = ExtensionIterator<QuiverMatrix>(class_.next());
  }
  return iter_.next();
}

bool
EquivMutClassExtIterator::has_next() {
  return iter_.has_next() || class_.has_next();
}
}  // namespace cluster
