/*
 * oriented_cycle_info.h
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
 * Contains the OrientedCycleInfo class. This stores information about the
 * cycles in a quiver and provides methods to check whether other instances are
 * the same. The orientation of the arrows in the quiver are considered, but a
 * cycle is any loop in the quiver disregarding the orientations.
 *
 * e.g. { { 0 1 -1 } { -1 0 -1 } { 1 1 0 } } would be considered to have a
 * cycle, but that cycle is different to the cycle in
 * { { 0 1 -1 } { -1 0 1 } { 1 -1 0 } }.
 */
#pragma once

#include "equiv_quiver_matrix.h"

namespace cluster {
class OrientedCycleInfo {
 public:
  /**
   * Construct the cycle information about the specified matrix.
   * @param matrix Matrix to extract cycle information from
   */
  OrientedCycleInfo(const QuiverMatrix& matrix);
  /**
   * Check whether two instances are equal.
   * @param cycle Instance to check
   * @return true if cycle is the same (up to permutation) as this
   */
  bool equals(const OrientedCycleInfo& cycle) const;
  /**
   * Calculate the has of the cycle information.
   * @return Hashcode of this information
   */
  std::size_t hash() const;
  /**
   * Output the matrix which stores the cycle information.
   */
  friend std::ostream& operator<<(std::ostream& os,
                                  const OrientedCycleInfo& cycle);

 private:
  /** Matrix containing only the cyclic part of the initial matrix. */
  EquivQuiverMatrix matrix_;
};
}  // namespace cluster

namespace std {
/* Add hash function to the std::hash struct. */
template <>
struct hash<cluster::OrientedCycleInfo> {
  size_t operator()(const cluster::OrientedCycleInfo& x) const {
    return x.hash();
  }
};
template <>
struct hash<std::shared_ptr<cluster::OrientedCycleInfo>> {
  size_t operator()(
      const std::shared_ptr<cluster::OrientedCycleInfo>& x) const {
    return x->hash();
  }
};
/* Add equals function to std::equal_to */
template <>
struct equal_to<cluster::OrientedCycleInfo> {
  bool operator()(const cluster::OrientedCycleInfo& lhs,
                  const cluster::OrientedCycleInfo& rhs) const {
    return lhs.equals(rhs);
  }
};
template <>
struct equal_to<std::shared_ptr<cluster::OrientedCycleInfo>> {
  bool operator()(
      const std::shared_ptr<cluster::OrientedCycleInfo>& lhs,
      const std::shared_ptr<cluster::OrientedCycleInfo>& rhs) const {
    return lhs->equals(*rhs);
  }
};
}  // namespace std
