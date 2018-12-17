/*
 * link_holder.cc
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
#include "link_holder.h"

#include "equiv_quiver_matrix.h"
#include "int_matrix.h"
#include "quiver_matrix.h"

namespace cluster {

template <class T>
LinkHolder<T>::LinkHolder() : links_(0, false), size_(0) {}

template <class T>
LinkHolder<T>::LinkHolder(const int size) : links_(size, false), size_(size) {}

template <class T>
LinkHolder<T>::~LinkHolder() {}

template <class T>
void
LinkHolder<T>::size(const int size) {
  size_ = size;
  links_.reserve(size);
  /* Reserve makes bools which are true. We want false. */
  for (int i = 0; i < size; i++) {
    links_[i] = false;
  }
}

template <class T>
void
LinkHolder<T>::matrix(std::shared_ptr<T> mat) {
  matrix_ = mat;
}

template <class T>
std::shared_ptr<T>
LinkHolder<T>::matrix() const {
  return matrix_;
}

template <class T>
void
LinkHolder<T>::link(const int k) {
  links_[k] = true;
}

template <class T>
bool
LinkHolder<T>::has_link(const int k) const {
  return links_[k];
}

template <class T>
bool
LinkHolder<T>::is_complete() const {
  for (int i = 0; i < size_; i++) {
    if (!links_[i]) {
      return false;
    }
  }
  return true;
}

template class LinkHolder<IntMatrix>;
template class LinkHolder<QuiverMatrix>;
template class LinkHolder<EquivQuiverMatrix>;

}  // namespace cluster
