/*
 * equiv_underlying_graph.h
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
 * Like the UnderlyingGraph this provides an easy way to view the underlying
 * graph of a quiver. EquivUnderlyingGraph has the same equals behaviour as
 * EquivQuiverMatrix, so graphs identical up to permuting vertices are equal.
 */
#pragma once

#include "equiv_quiver_matrix.h"

namespace cluster {
class EquivUnderlyingGraph : public EquivQuiverMatrix {

public:
  EquivUnderlyingGraph(const IntMatrix &);
  /** Override the virtual IntMatrix::set_matrix method */
  void set_matrix(const IntMatrix &mat);
};
} // namespace cluster
namespace std {
/* Add hash function to the std::hash struct. */
template <> struct hash<cluster::EquivUnderlyingGraph> {
  size_t operator()(const cluster::EquivUnderlyingGraph &x) const {
    return x.hash();
  }
};
/**
 * Annoyingly the standard shared_ptr hash function which passes the
 * function to the object in the pointer doesn't seem to work.
 */
template <> struct hash<std::shared_ptr<cluster::EquivUnderlyingGraph>> {
  size_t
  operator()(const std::shared_ptr<cluster::EquivUnderlyingGraph> &x) const {
    return x->hash();
  }
};
/* Add equals function to std::equal_to */
template <> struct equal_to<cluster::EquivUnderlyingGraph> {
  bool operator()(const cluster::EquivUnderlyingGraph &lhs,
                  const cluster::EquivUnderlyingGraph &rhs) const {
    return lhs.equals(rhs);
  }
};
template <> struct equal_to<std::shared_ptr<cluster::EquivUnderlyingGraph>> {
  bool
  operator()(const std::shared_ptr<cluster::EquivUnderlyingGraph> &lhs,
             const std::shared_ptr<cluster::EquivUnderlyingGraph> &rhs) const {
    return lhs->equals(*rhs);
  }
};
} // namespace std
