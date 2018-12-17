/*
 * cycle.h
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
 * Contains the class Cycle which provides a wrapper to hold a cycle that
 * appears in a quiver.
 */
#pragma once

#include <functional>
#include <memory>
#include <ostream>
#include <vector>

namespace cluster {

class Cycle {

public:
  /**
   * Default constructor creates an empty cycle.
   */
  Cycle();

  /**
   * Create a cycle with the provided values. The values will be cyclically
   * permuted so that the smallest value is the first one.
   * @param vec Vector of vertex labels which form the cycle
   */
  Cycle(const std::vector<int> &vec);

  /**
   * Create a cycle with part of a vector. the size tells how many of the
   * values from the vector should be used from the first.
   * @param vec Vector of vertex labels
   * @param size Number of elements in cycle
   */
  Cycle(const std::vector<int> &vec, const int size);

  /**
   * Check whether this cycle is equal to the one provided.
   * @param rhs Cycle to check if equal to
   * @return true if equal
   */
  bool equals(const Cycle &rhs) const;

  /**
   * Check whether this cycle is equivalent to the one provided when it is
   * permuted by the values given.
   * @param rhs Cycle to check
   * @param permutation Mappings from one vertex to another
   * @return true if equivalent
   */
  bool equals(const Cycle &rhs, const std::vector<int> &permutation) const;

  /**
   * Hash the cycle and return the hashcode.
   * @return The hashcode.
   */
  std::size_t hash() const;

  /**
   * Get the size of the cycle. That is the number of vertices in the cycle.
   */
  std::size_t size() const;

  /**
   * Check whether the cycle contains the specified vertex.
   * @param value Vertex index to check
   * @return true if vertex is in the cycle
   */
  bool contains(const int value) const;

  /**
   * Check whether the cycle contains the values in the order that is
   * specified by the pair.
   * @param pair The pair of integers to check
   * @return true if both number are in the cycle and in the same order
   */
  bool contains(const std::pair<int, int> &pair) const;

  /**
   * Need to overload == to allow unordered_sets of cycles to be compared.
   */
  friend bool operator==(const Cycle &lhs, const Cycle &rhs) {
    return lhs.equals(rhs);
  }

  friend std::ostream &operator<<(std::ostream &os, const Cycle &cycle);

private:
  /**
   * Container to hold the cycle information.
   */
  std::vector<int> cycle_;

  /**
   * Find the index in the vector of the smallest value stored in the vector.
   * @param vec Vector to check
   * @return Index of the smallest value
   */
  static std::size_t smallest_index(const std::vector<int> &vec);

  /**
   * Find the index in the vector of the smallest value stored in the first
   * size values in the vector.
   * @param vec Vector to check
   * @param size Number of values to check from the beginning of the vector
   * @return Index of the smallest value
   */
  static std::size_t smallest_index(const std::vector<int> &vec,
                                    const int size);
};
} // namespace cluster

namespace std {
/* Add hash function to the std::hash struct. */
template <> struct hash<cluster::Cycle> {
  size_t operator()(const cluster::Cycle &x) const { return x.hash(); }
};
/* Add equals function to std::equal_to */
template <> struct equal_to<cluster::Cycle> {
  bool operator()(const cluster::Cycle &lhs, const cluster::Cycle &rhs) const {
    return lhs.equals(rhs);
  }
};
template <> struct hash<std::shared_ptr<cluster::Cycle>> {
  size_t operator()(const std::shared_ptr<cluster::Cycle> &x) const {
    return x->hash();
  }
};
template <> struct equal_to<std::shared_ptr<cluster::Cycle>> {
  bool operator()(const std::shared_ptr<cluster::Cycle> &lhs,
                  const std::shared_ptr<cluster::Cycle> &rhs) const {
    return lhs->equals(*rhs);
  }
};
} // namespace std
