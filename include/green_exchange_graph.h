/*
 * green_exchange_graph.h
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
 * Specialises exchange graph to stop when it encounters a quiver which is known
 * not to have a maximal green sequence.
 */
#pragma once

#include "template_exchange_graph.h"

namespace cluster {
namespace green_exchange {
/**
 * A quiver which contains a cyclic triangle, where all the edges are multiple
 * arrows, cannot have a maximal green sequence. This checks whether the given
 * quiver contains such a triangle and returns true if it does not.
 */
/* Initially we read the matrix to determine which vertices are the sources of
 * multiple weight arrows, and then we can use this cache to try and determine
 * whether they form a triangle.
 *
 * The triangle check is done for each vertex. At one vertex we can follow an
 * arrow to get a second vertex, from which we can follow one of its arrows to
 * get a third vertex. In order to get a triangle this third vertex must then
 * have an arrow in its cache which leads back to the initial vertex.
 */
struct MultiArrowTriangleCheck {
  bool operator()(QuiverMatrix const *const mptr, int /*vertex*/) const {
    const int nrows = mptr->num_rows();
    const int ncols = mptr->num_cols();
    _vertex_cache.resize(nrows);
    for (int i = 0; i < nrows; ++i) {
      _vertex_cache[i].clear();
    }
    const int *row = mptr->data();
    for (int r = 0; r < nrows; ++r) {
      for (int c = 0; c < ncols; ++c) {
        if (*(row++) > 1) {
          _vertex_cache[r].push_back(c);
        }
      }
    }
    for (int i = 0; i < nrows; ++i) {
      auto ith_vertex = _vertex_cache[i];
      for (int next_vert : ith_vertex) {
        for (int third_vert : _vertex_cache[next_vert]) {
          if (std::find(_vertex_cache[third_vert].begin(),
                        _vertex_cache[third_vert].end(),
                        i) != _vertex_cache[third_vert].end()) {
            return false;
          }
        }
      }
    }
    return true;
  }
  bool operator()(Seed const *const sptr, int vertex) const {
    return operator()(&(sptr->matrix()), vertex);
  }
  bool operator()(LabelledSeed const *const sptr, int vertex) const {
    return operator()(&(sptr->matrix()), vertex);
  }
  mutable std::vector<std::vector<int>> _vertex_cache;
};
using GreenGraphInfo =
    exchange_graph::detail::GraphInfo<MultiArrowTriangleCheck,
                                      exchange_graph::detail::NeverStop>;
template <class M>
using GreenVertexInfo = exchange_graph::detail::QuiverVertex<M>;
template <class M>
using GreenGraph = exchange_graph::Graph<GreenVertexInfo<M>, GreenGraphInfo>;
} // namespace green_exchange
typedef green_exchange::GreenGraph<Seed> GreenExchangeGraph;
typedef green_exchange::GreenGraph<LabelledSeed> GreenLabelledExchangeGraph;
typedef green_exchange::GreenGraph<QuiverMatrix> GreenLabelledQuiverGraph;
typedef green_exchange::GreenGraph<EquivQuiverMatrix> GreenQuiverGraph;
} // namespace cluster
