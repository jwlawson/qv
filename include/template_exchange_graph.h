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

template <class VertexInfo, class GraphInfo> class __TExchangeGraph {
public:
  typedef typename VertexInfo::vertex_value Vertex;
  typedef Vertex *VertexPtr;
  typedef Vertex const *CVertexPtr;
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
  typedef std::pair<Vertex, Link> value_type;
  typedef std::unordered_map<CVertexPtr, Link, VertexHash, VertexEquiv>
      GraphMap;

  /** Construct an empty graph which contains nothing. */
  __TExchangeGraph() = default;
  /** Construct the exchange graph for the specified matrix. */
  __TExchangeGraph(Vertex const &mat, int mat_size, size_t max_num = SIZE_MAX);
  /** Delete any matrix pointers controlled by this graph. */
  ~__TExchangeGraph();
  const typename GraphMap::const_iterator begin() const;
  const typename GraphMap::const_iterator end() const;

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
   * the ::operator++ until ::end() returns true.
   */
  class _GraphLoader {
  public:
    _GraphLoader() = default;

    _GraphLoader(__TExchangeGraph &graph);
    /** Load the next section of the graph. */
    void load_next();
    /** Return whether the whole graph been loaded. */
    bool end() const;

  private:
    __TExchangeGraph &_graph;
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
    CVertexPtr &operator[](int i);
    const typename LinkVec::const_iterator begin() const;
    const typename LinkVec::const_iterator end() const;
    /** Inital matrix */
    CVertexPtr _matrix;

  private:
    LinkVec _links;
  };
};
/* ExchangeGraph */
template <class V, class G>
inline __TExchangeGraph<V, G>::__TExchangeGraph(Vertex const &mat,
                                                int matrix_size, size_t max_num)
    : _matrix_size(matrix_size), _max_num(max_num) {
  VertexPtr m = new Vertex(mat);
  _queue.push_back(m);
  _map.emplace(m, Link(_queue.front(), _matrix_size));
  _GraphLoader l(*this);
  while (!l.end()) {
    l.load_next();
  }
}
template <class V, class G> inline __TExchangeGraph<V, G>::~__TExchangeGraph() {
  for (auto it = _map.begin(); it != _map.end(); ++it) {
    delete it->first;
  }
}
template <class V, class G>
inline const typename __TExchangeGraph<V, G>::GraphMap::const_iterator
__TExchangeGraph<V, G>::begin() const {
  return _map.begin();
}
template <class V, class G>
inline const typename __TExchangeGraph<V, G>::GraphMap::const_iterator
__TExchangeGraph<V, G>::end() const {
  return _map.end();
}
/* _GraphLoader */
template <class V, class G>
inline __TExchangeGraph<V, G>::_GraphLoader::_GraphLoader(
    __TExchangeGraph &graph)
    : _graph(graph), _size(graph._matrix_size) {}

template <class V, class G>
inline void __TExchangeGraph<V, G>::_GraphLoader::load_next() {
  CVertexPtr mat = _graph._queue.front();
  _graph._queue.pop_front();
  for (std::size_t i = 0; !_end && i < _size; ++i) {
    if (mutate_at(mat, i)) {
      VertexPtr new_matrix = new_instance(_size);
      mutate(mat, i, *new_matrix);
      if (have_seen(new_matrix)) {
        seen_matrix(new_matrix, mat, i);
        delete new_matrix;
      } else {
        // up to this point _end is always false
        _end = should_stop(new_matrix);
        unseen_matrix(new_matrix, mat, i);
      }
    }
  }
  _end = _end || _graph._queue.empty();
}
template <class V, class G>
inline bool __TExchangeGraph<V, G>::_GraphLoader::end() const {
  return _end;
}
template <class V, class G>
inline bool
__TExchangeGraph<V, G>::_GraphLoader::have_seen(CVertexPtr new_mat) {
  return _graph._map.find(new_mat) != _graph._map.end();
}
template <class V, class G>
inline bool __TExchangeGraph<V, G>::_GraphLoader::mutate_at(CVertexPtr old_mat,
                                                            int vertex) {
  bool result = should_continue(old_mat, vertex);
  if (result) {
    auto position = _graph._map.find(old_mat);
    result =
        position != _graph._map.end() && position->second[vertex] == nullptr;
  }
  return result;
}
template <class V, class G>
inline void __TExchangeGraph<V, G>::_GraphLoader::seen_matrix(
    CVertexPtr new_mat, CVertexPtr old_mat, int vertex) {
  auto ref = _graph._map.find(new_mat);
  if (ref != _graph._map.end() && is_exactly(new_mat, ref->first)) {
    ref->second[vertex] = old_mat;
  }
  auto &old_matrix_vertex_link = _graph._map[old_mat][vertex];
  if (old_matrix_vertex_link == nullptr) {
    old_matrix_vertex_link = ref->first;
  }
}
template <class V, class G>
inline void __TExchangeGraph<V, G>::_GraphLoader::unseen_matrix(
    CVertexPtr new_mat, CVertexPtr old_mat, int vertex) {
  _graph._map.emplace(new_mat, _Link(new_mat, _size));
  _graph._map[new_mat][vertex] = old_mat;
  _graph._map[old_mat][vertex] = new_mat;
  _graph._queue.push_back(new_mat);
}
/* _Link */
template <class V, class G>
inline __TExchangeGraph<V, G>::_Link::_Link() : _matrix(nullptr) {}

template <class V, class G>
inline __TExchangeGraph<V, G>::_Link::_Link(CVertexPtr matrix, int size)
    : _matrix(matrix), _links(size, nullptr) {}

template <class V, class G>
inline typename __TExchangeGraph<V, G>::CVertexPtr &
    __TExchangeGraph<V, G>::_Link::operator[](int i) {
  return _links[i];
}
template <class V, class G>
inline const typename __TExchangeGraph<V, G>::_Link::LinkVec::const_iterator
__TExchangeGraph<V, G>::_Link::begin() const {
  return _links.begin();
}
template <class V, class G>
inline const typename __TExchangeGraph<V, G>::_Link::LinkVec::const_iterator
__TExchangeGraph<V, G>::_Link::end() const {
  return _links.end();
}

template <class Vertex, class Hash, class Equiv, class Equal, class Mutate,
          class NewInstance>
struct VertexInfo {
  typedef Vertex vertex_value;
  typedef Hash hash_value;
  typedef Equiv equiv_value;
  typedef Equal equals_value;
  typedef Mutate mutate_value;
  typedef NewInstance newinstance_value;
};

template <class ShouldMutate, class ShouldStop> struct GraphInfo {
  typedef ShouldMutate shouldmutate_value;
  typedef ShouldStop shouldstop_value;
};

namespace _EGDefaults {
struct AlwaysMutate {
  bool operator()(void const *const /* ignored */,
                  int const /* ignored */) const {
    return true;
  }
};
template <class M> struct Equiv {
  bool operator()(M const *const lhs, M const *const rhs) const {
    return lhs->equals(*rhs);
  }
};
template <class M> struct Hash {
  bool operator()(M const *const m) const { return m->hash(); }
};
struct IsExactly {
  bool operator()(QuiverMatrix const *const, QuiverMatrix const *const) {
    return true;
  }
  bool operator()(EquivQuiverMatrix const *const lhs,
                  EquivQuiverMatrix const *const rhs) {
    return IntMatrix::are_equal(*lhs, *rhs);
  }
  bool operator()(Seed const *const lhs, Seed const *const rhs) {
    return IntMatrix::are_equal(lhs->matrix(), rhs->matrix()) &&
           (lhs->cluster() == rhs->cluster());
  }
  bool operator()(LabelledSeed const *const /* ignored */,
                  LabelledSeed const *const /* ignored */) {
    return true;
  }
};
template <class M> struct Mutate {
  void operator()(M const *const m, int vertex, M &out) const {
    m->mutate(vertex, out);
  }
};
struct NeverStop {
  bool operator()(void const *const /*ignored */) { return false; }
};
template <class M, class Enable = void> struct GetInstance {
  M *operator()(std::size_t const size) const { return new M(size); }
};
template <class M>
struct GetInstance<
    M, typename std::enable_if<std::is_base_of<QuiverMatrix, M>::value>::type> {
  M *operator()(std::size_t const size) const { return new M(size, size); }
};
template <class M>
using QuiverVertex =
    VertexInfo<M, Hash<M>, Equiv<M>, IsExactly, Mutate<M>, GetInstance<M>>;
using ComputeAllGraph = GraphInfo<AlwaysMutate, NeverStop>;
template <class M>
using EGAlways = __TExchangeGraph<QuiverVertex<M>, ComputeAllGraph>;
}
typedef _EGDefaults::EGAlways<Seed> ExchangeGraph;
typedef _EGDefaults::EGAlways<LabelledSeed> LabelledExchangeGraph;
typedef _EGDefaults::EGAlways<QuiverMatrix> LabelledQuiverGraph;
typedef _EGDefaults::EGAlways<EquivQuiverMatrix> QuiverGraph;
}
