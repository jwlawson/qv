/**
 * stream_extension_iterator.cc
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
#include "stream_extension_iterator.h"

#include <iostream> /* Need iostream for std::cerr */

#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {

template <class T>
StreamExtIterator<T>::StreamExtIterator() : stream_iter_(), ext_iter_() {}

template <class T>
StreamExtIterator<T>::StreamExtIterator(std::istream& istream)
    : stream_iter_(istream), ext_iter_() {}

template <class T>
T const&
StreamExtIterator<T>::next() {
  if (!ext_iter_.has_next()) {
    ext_iter_ = ExtensionIterator<T>(*stream_iter_.next());
  }
  return ext_iter_.next();
}

template <class T>
bool
StreamExtIterator<T>::has_next() {
  return ext_iter_.has_next() || stream_iter_.has_next();
}

template class StreamExtIterator<QuiverMatrix>;
template class StreamExtIterator<EquivQuiverMatrix>;
}  // namespace cluster
