/*
 * cycle_info.h
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
 * Contains the CycleInfo class. This extracts the cycles in a quiver and stores
 * this information. There are then methods to compare the cycle information for
 * different quivers.
 *
 * CycleInfo does not take into account the direction that the arrows in the
 * quiver are oriented.
 */
#pragma once

#include <unordered_set>

#include "cycle.h"
#include "quiver_matrix.h"

namespace cluster {

class CycleInfo {
 private:
  /**
   * Double edges in the quivers need to be tracked, so keep them as pairs
   * with the first int the index of the initial vertex, the second is the
   * index of the receiving vertex.
   *
   * i.e. first ==> second
   */
  typedef std::pair<int, int> DEdge;

  struct PairHash {
    std::size_t operator()(const DEdge& key) const {
      return 31 * (key.first + 31 * key.second);
    }
  };

 public:
  /**
   * Default constructor.
   */
  CycleInfo();

  /**
   * Create a CycleInfo instance containing the cycles in the supplied
   * matrix.
   * @param matrix The matrix to extract cycle information from
   */
  CycleInfo(const QuiverMatrix& matrix);

  /**
   * Check whether any cycles contain the specified vertex.
   * @param vertex Vertex to check
   * @return true if any cycle contains the vertex
   */
  bool cycle_contains(const int vertex) const;

  /**
   * Check whether the cycles in the supplied CycleInfo instance are the same
   * as those in this one.
   * @param rhs CycleInfo instance to check
   * @return true if the cycles are the same
   */
  bool equals(const CycleInfo& rhs) const;

  /**
   * Calculate the hashcode.
   * @return hashcode
   */
  std::size_t hash() const;

 private:
  /** Number of vertices in the quiver. */
  const int size_;

  /** Vector containing all cycles in the quiver. */
  std::unordered_set<Cycle> cycles_;

  /**
   * Vector counting the number of cycles of each length.
   * The int at index i is the number of cycles of length i.
   */
  std::vector<int> num_cycles_;

  /** Vector containing all double edges in the cycles. */
  std::unordered_set<DEdge, PairHash> edges_;

  /**
   * Calculate all cycles in the matrix.
   * @param matrix Matrix to check.
   */
  void comp_cycles(const QuiverMatrix& matrix);

  /**
   * Find all double edges in the cycles.
   * @param matrix Matrix to check.
   */
  void find_double_edges(const QuiverMatrix& matrix);

  /**
   * Find all the cycles in the matrix.
   *
   * Recursively search for all cycles and when found add them to the set of
   * cycles.
   * @param vertex Vertex to start search from
   * @param cycle Vector containing the cycle so far
   * @param index Index where vertex is in the cycle
   * @param matrix Matrix to check for cycles
   */
  void cycles_from(int vertex, std::vector<int>& cycle, int index,
                   const QuiverMatrix& matrix);

  /**
   * Check whether the vector contains the specified value in the first size
   * terms.
   * @param vec Vector to check
   * @param value Value to find
   * @param size Number of indices from the start to check
   * @return true if the calue is in the vector
   */
  static bool vector_contains(const std::vector<int>& vec, const int value,
                              const int size);

  bool perm_equals(const std::vector<std::vector<int>>& maps,
                   const CycleInfo& rhs, std::vector<int>& so_far,
                   int index) const;
};
}  // namespace cluster

namespace std {
/* Add hash function to the std::hash struct. */
template <>
struct hash<cluster::CycleInfo> {
  size_t operator()(const cluster::CycleInfo& x) const { return x.hash(); }
};
template <>
struct hash<std::shared_ptr<cluster::CycleInfo>> {
  size_t operator()(const std::shared_ptr<cluster::CycleInfo>& x) const {
    return x->hash();
  }
};
/* Add equals function to std::equal_to */
template <>
struct equal_to<cluster::CycleInfo> {
  bool operator()(const cluster::CycleInfo& lhs,
                  const cluster::CycleInfo& rhs) const {
    return lhs.equals(rhs);
  }
};
template <>
struct equal_to<std::shared_ptr<cluster::CycleInfo>> {
  bool operator()(const std::shared_ptr<cluster::CycleInfo>& lhs,
                  const std::shared_ptr<cluster::CycleInfo>& rhs) const {
    return lhs->equals(*rhs);
  }
};
}  // namespace std
