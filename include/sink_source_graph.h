/*
 * sink_source_graph.h
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
#pragma once

#include "exchange_graph.h"

namespace cluster {

namespace _sinksource {
struct OnlyMutateSinkSource {
  bool operator()(IntMatrix const &matrix, int vertex) const {
    int const *const row = matrix.data() + vertex * matrix.num_cols();
    bool all_pos = true;
    bool all_neg = true;
    for (int i = 0, max = matrix.num_cols(); i < max && (all_pos || all_neg);
         ++i) {
      all_neg = all_neg && row[i] <= 0;
      all_pos = all_pos && row[i] >= 0;
    }
    return all_pos || all_neg;
  }
  bool operator()(IntMatrix const *const matrix, int vertex) const {
    return operator()(*matrix, vertex);
  }
  template <class M>
  bool operator()(__Seed<M> const *const seed, int vertex) const {
    return operator()(seed->matrix(), vertex);
  }
};
} // namespace _sinksource
template <class M>
using SinkSourceGraph = __ExchangeGraph<M, _sinksource::OnlyMutateSinkSource>;

} // namespace cluster
