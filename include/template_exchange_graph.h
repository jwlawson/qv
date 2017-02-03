/*
 * exchange_graph.h
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
 * Computes an entire quiver graph and keeps all quivers in a map. This is
 * probably slower than the mutation_class equivalent, but those get rid of
 * unneeded quivers.
 *
 * This templated version allows the user to fully configure the exchange graph.
 * Two parameters are set: VertexInfo which contains information about the
 * vertices in the graph; and GraphInfo which contains information about the
 * construction of the graph itself.
 *
 * VertexInfo must contain the following types:
 *
 *   VertexInfo::vertex_value
 *     The type of the vertex in the graph
 *
 *   VertexInfo::hash_value
 *     A functor to compute a hash of a vertex, containing function
 *     size_t operator()(Vertex const *) const
 *
 *   VertexInfo::equiv_value
 *     A functor to determine whether two vertex objects should be considered
 *     equivalent in the graph, and so represent the same vertex in the graph
 *     bool operator()(Vertex const * const, Vertex const * const) const
 *
 *   VertexInfo::equals_value
 *     A functor to determine whether two equivalent vertex objects should be
 *     considered exaclty the same. This is used to ensure that even if two
 *     isomorphic quivers represent the same vertex, the permutation between
 *     them does not skew which mutation label is stored in the graph.
 *     bool operator()(Vertex const * const, Vertex const * const) const
 *
 *   VertexInfo::mutate_value
 *     A functor to compute the mutation of a vertex in a specific direction.
 *     void operator()(Vertex const * const in, int dir, Vertex& out) const
 *
 *   VertexInfo::newinstance_value
 *     A functor to return a new instance of a Vertex object. The graph will
 *     take ownership of the instance.
 *     Vertex* operator()(size_t const size) const
 *
 * GraphInfo must contain the following types:
 *
 *   GraphInfo::shouldmutate_value
 *     A functor which determines whether a given Vertex should be mutated in a
 *     particular direction. If true, then the mutation is computed and added to
 *     the graph, otherwise it is skipped.
 *     bool operator()(Vertex const * const, int const dir) const
 *
 *   GraphInfo::shouldstop_value
 *     A functor which determines whether the graph computation should be
 *     stopped. Use this if you only need to compute the first instance of a
 *     certain Vertex, or a Vertex with a certain property, in the graph.
 *     Note, this is in addition to the graph ctor parameter which will
 *     stop the graph once a certain number of vertices have been computed.
 *     bool operator()(Vertex const * const) const
 */
#pragma once

#include <algorithm>
#include <deque>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "seed.h"

namespace cluster {
namespace exchange_graph {
template <class VertexInfo, class GraphInfo>
class Graph {
 public:
  typedef typename VertexInfo::vertex_value Vertex;
  typedef Vertex* VertexPtr;
  typedef Vertex const* CVertexPtr;
  typedef typename VertexInfo::hash_value VertexHash;
  typedef typename VertexInfo::equiv_value VertexEquiv;
  typedef typename VertexInfo::equals_value VertexEqual;
  typedef typename VertexInfo::mutate_value Mutate;
  typedef typename VertexInfo::newinstance_value NewVertex;

  typedef typename GraphInfo::shouldmutate_value ShouldMutate;
  typedef typename GraphInfo::shouldstop_value ShouldStop;

 private:
  class _GraphLoader;
  class _Link;

 public:
  typedef _GraphLoader loader;
  typedef _Link Link;
  typedef std::unordered_map<CVertexPtr, Link, VertexHash, VertexEquiv>
      GraphMap;

  /** Construct an empty graph which contains nothing. */
  Graph() = default;
  /** Construct the exchange graph for the specified matrix. */
  Graph(Vertex const& mat, int mat_size, size_t max_num = SIZE_MAX);
  /** Delete any matrix pointers controlled by this graph. */
  ~Graph();
  const typename GraphMap::const_iterator begin() const;
  const typename GraphMap::const_iterator end() const;
	/** Get a reference to the underlying map which holds the graph. */
	GraphMap const& underlying_map() const;

 private:
  /** Size of matrix. */
  const size_t _matrix_size;
  /** Maximum number of entries in the graph. */
  const size_t _max_num;
  /** Map of matrices and their links. */
  GraphMap _map;
  /** Queue of matrices to mutate next. */
  std::deque<CVertexPtr> _queue;
  /**
   * GraphLoader actually computes the whole graph. The Graph ctor loops on
   * load_next() until end() returns true.
   */
  class _GraphLoader {
   public:
    _GraphLoader() = default;

    _GraphLoader(Graph& graph);
    /** Load the next section of the graph. */
    void load_next();
    /** Return whether the whole graph been loaded. */
    bool end() const;

   private:
    Graph& _graph;
    bool _end = false;
    size_t _size;
    VertexEqual is_exactly;
    VertexEquiv is_equivalent;
    Mutate mutate;
    NewVertex new_instance;
    ShouldMutate should_continue;
    ShouldStop should_stop;

    bool have_seen(CVertexPtr new_mat);
    bool mutate_at(CVertexPtr old_mat, int vertex);
    void seen_matrix(CVertexPtr new_mat, CVertexPtr old_mat, int vertex);
    void unseen_matrix(CVertexPtr new_mat, CVertexPtr old_mat, int vertex);
  };
  /**
   * Link stores the edges adjacent to its matrix.
   */
  class _Link {
    typedef std::vector<CVertexPtr> LinkVec;

   public:
    /** Construct link for null matrix. */
    _Link();
    /** Construct link for given matrix. */
    _Link(CVertexPtr matrix, int size);
    /** Get link at i */
    CVertexPtr& operator[](int i);
    const typename LinkVec::const_iterator begin() const;
    const typename LinkVec::const_iterator end() const;
    /** Inital matrix */
    CVertexPtr _matrix;

   private:
    LinkVec _links;
  };
};
namespace detail {
template <class Vertex,
          class Hash,
          class Equiv,
          class Equal,
          class Mutate,
          class NewInstance>
struct VertexInfo {
  typedef Vertex vertex_value;
  typedef Hash hash_value;
  typedef Equiv equiv_value;
  typedef Equal equals_value;
  typedef Mutate mutate_value;
  typedef NewInstance newinstance_value;
};

template <class ShouldMutate, class ShouldStop>
struct GraphInfo {
  typedef ShouldMutate shouldmutate_value;
  typedef ShouldStop shouldstop_value;
};
struct AlwaysMutate {
  bool operator()(void const* const /* ignored */,
                  int const /* ignored */) const {
    return true;
  }
};
template <class M>
struct Equiv {
  bool operator()(M const* const lhs, M const* const rhs) const {
    return lhs->equals(*rhs);
  }
};
template <class M>
struct Hash {
  bool operator()(M const* const m) const { return m->hash(); }
};
struct IsExactly {
  bool operator()(QuiverMatrix const* const, QuiverMatrix const* const) {
    return true;
  }
  bool operator()(EquivQuiverMatrix const* const lhs,
                  EquivQuiverMatrix const* const rhs) {
    return IntMatrix::are_equal(*lhs, *rhs);
  }
  bool operator()(Seed const* const lhs, Seed const* const rhs) {
    return IntMatrix::are_equal(lhs->matrix(), rhs->matrix()) &&
           (lhs->cluster() == rhs->cluster());
  }
  bool operator()(LabelledSeed const* const /* ignored */,
                  LabelledSeed const* const /* ignored */) {
    return true;
  }
};
template <class M>
struct Mutate {
  void operator()(M const* const m, int vertex, M& out) const {
    m->mutate(vertex, out);
  }
};
struct NeverStop {
  bool operator()(void const* const /*ignored */) { return false; }
};
template <class M, class Enable = void>
struct GetInstance {
  M* operator()(std::size_t const size) const { return new M(size); }
};
template <class M>
struct GetInstance<
    M,
    typename std::enable_if<std::is_base_of<QuiverMatrix, M>::value>::type> {
  M* operator()(std::size_t const size) const { return new M(size, size); }
};
template <class M>
using QuiverVertex =
    VertexInfo<M, Hash<M>, Equiv<M>, IsExactly, Mutate<M>, GetInstance<M>>;
using ComputeAllGraph = GraphInfo<AlwaysMutate, NeverStop>;
template <class M>
using QuiverEG = Graph<QuiverVertex<M>, ComputeAllGraph>;
}
}
typedef exchange_graph::detail::QuiverEG<Seed> ExchangeGraph;
typedef exchange_graph::detail::QuiverEG<LabelledSeed> LabelledExchangeGraph;
typedef exchange_graph::detail::QuiverEG<QuiverMatrix> LabelledQuiverGraph;
typedef exchange_graph::detail::QuiverEG<EquivQuiverMatrix> QuiverGraph;
}
#include "template_exchange_graph.impl"

