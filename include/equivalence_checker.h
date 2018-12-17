/*
 * equivalence_checker.h
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
 * Contains the EquivalenceChecker class which checks whether two matrices are
 * equivalent up to permuting their rows and columns.
 */
#pragma once

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

#include "equiv_quiver_matrix.h"

namespace cluster {
class EquivalenceChecker {
 private:
  typedef EquivQuiverMatrix M;
  typedef std::vector<int> Permutation;
  typedef std::shared_ptr<std::vector<Permutation>> PermVecPtr;

 public:
  /**
   * Get the singleton instance of EquivalenceChecker.
   * This is less important now that we are not creating every possible
   * permutation, but still reduces the amount of allocations.
   */
  static std::shared_ptr<EquivalenceChecker> Get(const int size);

  /** Create empty checker that is essentially useless. */
  EquivalenceChecker();
  /**
   * Create instance with pre-allocated vectors of the required size.
   * @param size Size of matrices to be checked
   */
  EquivalenceChecker(const int size);
  /**
   * Check if the two matrices are equivalent up to a permutation of their
   * rows and columns.
   * @param lhs First matrix
   * @param rhs Second matrix
   * @return true if matrices are equivalent
   */
  bool are_equivalent(const M& lhs, const M& rhs);
  /** Get the mapping used in last equals check. */
  const Permutation& last_row_map() const;
  /**
   * Get all valid permutations from lhs to rhs.
   *
   * This method assumes that the two matrices are equivalent, so skips some
   * checks that are_equivalent does. This could cause problems if
   * are_equivalent is not checked before running this method.
   * @param lhs Matrix to get permutations from
   * @param rhs Matrix to get permutations to
   * @return vector of vectors representing the permutations
   */
  PermVecPtr valid_row_maps(const M& lhs, const M& rhs);

 private:
  /**
   * Stores information about possible mappings between the matrices.
   */
  class Mapping {
   public:
    Mapping(int size) : row_mappings(size) {}
    /**
     * Reset all mapping information.
     */
    void reset();
    /**
     * Update the mapping at a_index to include a link to b_index.
     */
    void update_row_mapping(const int a_index, const int b_index);
    /** Actual mapping data */
    std::vector<std::vector<int>> row_mappings;
  };
  /** Size of matrices to check. */
  int size_;
  /** Info about the possible permutations between the specified matrices. */
  Mapping maps_;
  /** Mapping used in last equals check run. */
  std::vector<int> last_row_map_;

  /** Check whether the rows of the matrices match. */
  bool do_rows_match(const M& a, const M& b);
  /**
   * Check whether the two matrices are permutations by considering the
   * mappings calculated earlier. Uses recursion on the index.
   */
  bool check_perm(Permutation& row_map, int index, const M& a, const M& b);
  /**
   * Run through all permutations and add any valid ones to the PermVecPtr
   * parameter.
   */
  void all_perms(Permutation& row_map, int index, const M& a, const M& b,
                 PermVecPtr perms);
};
inline const EquivalenceChecker::Permutation&
EquivalenceChecker::last_row_map() const {
  return last_row_map_;
}
inline void
EquivalenceChecker::Mapping::reset() {
  std::for_each(row_mappings.begin(), row_mappings.end(),
                [](std::vector<int>& v) { v.resize(0); });
}
inline void
EquivalenceChecker::Mapping::update_row_mapping(const int a_index,
                                                const int b_index) {
  row_mappings[a_index].push_back(b_index);
}
}  // namespace cluster
