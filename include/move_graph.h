/*
 * move_graph.h
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
 * Graph containing all matrices in a move-class and the moves which link them.
 *
 * The matrices are considered up to sink-source mutations.
 */
#pragma once

#include <deque>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "mmi_move.h"

namespace cluster {

template <class Matrix>
class MoveGraph {
  typedef Matrix* MatrixPtr;
  typedef Matrix const* CMatrixPtr;

 private:
  class _GraphLoader;
  class _Link;
  /** Checks whether the matrices are equal, rather than the pointers */
  struct PtrEqual {
    bool operator()(CMatrixPtr const& lhs, CMatrixPtr const& rhs) const {
      return lhs->equals(*rhs);
    }
  };
  /** Gets the matrix hash */
  struct PtrHash {
    size_t operator()(CMatrixPtr const& ptr) const { return ptr->hash(); }
  };

 public:
  typedef _GraphLoader loader;
  typedef _Link Link;
  typedef std::pair<Matrix, Link> value_type;
  typedef std::vector<MMIMove> MoveVec;
  typedef std::unordered_map<CMatrixPtr, Link, PtrHash, PtrEqual> GraphMap;

  /**
   * Create an instance of an empty graph. With no inital matrix the graph
   * will never contain anything.
   */
  MoveGraph() = default;
  /**
   * Compute the move-class for the provided matrix which uses the moves
   * provided.
   *
   * All computations are done in the constructor - so once constructed the
   * class is fully formed, but it does take a while for the largest classes.
   */
  MoveGraph(const Matrix& mat, MoveVec moves);
  ~MoveGraph() {
    for (auto it = _map.begin(); it != _map.end(); ++it) {
      delete it->first;
    }
  }
  const typename GraphMap::const_iterator begin() const { return _map.begin(); }
  const typename GraphMap::const_iterator end() const { return _map.end(); }

 private:
  /** Initial matrix */
  const Matrix& _matrix;
  /** Map storing the links related to each matrix. */
  GraphMap _map;
  /** Queue of matrices to compute moves on */
  std::deque<MatrixPtr> _queue;
  /** Vector of all moves to use to compute the class. */
  MoveVec _moves;

  CMatrixPtr ss_move_equiv(CMatrixPtr mat);
  void add_ss_equiv(CMatrixPtr mat);

  /**
   * Class which actually computes the class. cf MutationClassLoader
   *
   * Originally planned to follow the c++ iterator style, but that didn't fit
   * with the computations. Instead the whole class is computed by repeatedly
   * calling operator++ until end() returns true.
   *
   * The design is a little confused. The matrix queue should probably be a
   * member of _GraphLoader rather than MoveGraph.
   */
  class _GraphLoader {
   public:
    _GraphLoader() = default;

    _GraphLoader(MoveGraph& graph)
        : _graph(graph), _size(_graph._matrix.num_rows()) {}
    /** Compute all moves for the next matrix in the queue. */
    loader& operator++();
    /** Check whether the whole class is laoded. Returns true when it is. */
    bool end() const { return _end; }

   private:
    MoveGraph& _graph;
    bool _end = false;
    int _size;

    void seen_matrix(CMatrixPtr new_mat, CMatrixPtr old_mat);
    void unseen_matrix(CMatrixPtr const& new_mat, CMatrixPtr old_mat);
  };
  /**
   * Stores the edges between vertices in the graph.
   *
   * Each matrix M has one _Link associated to it, which contains references to
   * each matrix which is connected to M by an edge.
   */
  class _Link {
   private:
    typedef std::unordered_set<CMatrixPtr, PtrHash, PtrEqual> Set;

   public:
    _Link() : _matrix(nullptr){};
    _Link(CMatrixPtr matrix) : _matrix(matrix) {}
    /** Initial matrix */
    const CMatrixPtr _matrix;
    /** Add matrix which is linked to the initial matrix. */
    void add_link(CMatrixPtr matrix) { _links.insert(matrix); }
    const typename Set::const_iterator begin() const { return _links.begin(); }
    const typename Set::const_iterator end() const { return _links.end(); }

   private:
    Set _links;
  };
};
}  // namespace cluster
