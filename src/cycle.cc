/*
 * cycle.cc
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
#include "cycle.h"

namespace cluster {

/* Default ctr does nothing really. */
Cycle::Cycle() : cycle_() {}

Cycle::Cycle(const std::vector<int>& vec) : cycle_() {
  using std::size_t;
  cycle_.reserve(vec.size());
  size_t index = smallest_index(vec);
  for (size_t i = index; i < vec.size(); ++i) {
    cycle_.push_back(vec[i]);
  }
  for (size_t i = 0; i < index; ++i) {
    cycle_.push_back(vec[i]);
  }
}

Cycle::Cycle(const std::vector<int>& vec, const int size) : cycle_() {
  using std::size_t;
  cycle_.reserve(size);
  size_t index = smallest_index(vec, size);
  for (size_t i = index; i < (size_t)size; ++i) {
    cycle_.push_back(vec[i]);
  }
  for (size_t i = 0; i < index; ++i) {
    cycle_.push_back(vec[i]);
  }
}

bool
Cycle::equals(const Cycle& rhs) const {
  return cycle_ == rhs.cycle_;
}

bool
Cycle::equals(const Cycle& rhs, const std::vector<int>& permutation) const {
  if (cycle_.size() != rhs.cycle_.size()) {
    return false;
  }
  using std::size_t;
  std::vector<int> mapped(cycle_.size());
  for (size_t i = 0; i < cycle_.size(); ++i) {
    mapped[i] = permutation[rhs.cycle_[i]];
  }
  Cycle tmp(mapped);
  return equals(tmp);
}

std::size_t
Cycle::hash() const {
  using std::size_t;
  size_t hash = 1;
  for (size_t i = 0; i < cycle_.size(); ++i) {
    hash *= 47;
    hash += cycle_[i];
  }
  return hash;
}

std::size_t
Cycle::size() const {
  return cycle_.size();
}

bool
Cycle::contains(const int value) const {
  using std::size_t;
  for (size_t i = 0; i < cycle_.size(); ++i) {
    if (cycle_[i] == value) {
      return true;
    }
  }
  return false;
}

bool
Cycle::contains(const std::pair<int, int>& pair) const {
  using std::size_t;
  if (cycle_.empty()) {
    return false;
  }
  if (cycle_[0] == pair.second) {
    /* Pair is wrapped around the cycle. */
    return cycle_[cycle_.size() - 1] == pair.first;
  }
  for (size_t i = 0; i < cycle_.size() - 1; ++i) {
    if (cycle_[i] == pair.first && cycle_[i + 1] == pair.second) {
      return true;
    }
  }
  return false;
}

std::size_t
Cycle::smallest_index(const std::vector<int>& vec) {
  using std::size_t;
  int smallest = vec[0];
  size_t index = 0;
  for (size_t i = 1; i < vec.size(); ++i) {
    if (vec[i] < smallest) {
      smallest = vec[i];
      index = i;
    }
  }
  return index;
}

std::size_t
Cycle::smallest_index(const std::vector<int>& vec, const int size) {
  using std::size_t;
  int smallest = vec[0];
  size_t index = 0;
  for (size_t i = 1; i < (size_t)size; ++i) {
    if (vec[i] < smallest) {
      smallest = vec[i];
      index = i;
    }
  }
  return index;
}

std::ostream&
operator<<(std::ostream& os, const Cycle& cycle) {
  for (std::size_t i = 0; i < cycle.cycle_.size(); ++i) {
    os << cycle.cycle_[i] << " ";
  }
  return os;
}

}  // namespace cluster
