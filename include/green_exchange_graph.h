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

#include "exchange_graph.h"

namespace cluster {
namespace _green_exchange {
struct InfiniteTypeSink {
	bool operator()(QuiverMatrix const * const mptr, int /*vertex*/) const {
		for(int i = 0; i < mptr->num_rows(); ++i) {
			_vertex_cache[i].clear();
		}
		const int * row = mptr->data();
		const int nrows = mptr->num_rows();
		const int ncols = mptr->num_cols();
		for(int c = 0, r = 0; r < nrows; ++r) {
			for(c = 0; c < ncols; ++c) {
				if(*(row++) > 1) {
					_vertex_cache[r].push_back(c);
				}
			}
		}
		for(auto pair : _vertex_cache) {
			for(int next_vert : pair.second) {
				for(int third_vert : _vertex_cache[next_vert]) {
					if(std::find(_vertex_cache[third_vert].begin(),
								_vertex_cache[third_vert].end(), pair.first) !=
							_vertex_cache[third_vert].end()) {
						return false;
					}
				}
			}
		}
		/*
		const int * row_start = mptr->data() + (vertex*mptr->num_cols());
		for(int i = 0; i < mptr->num_cols(); ++i) {
			if(row_start[i] < -1) return false;
		}
		*/
		return true;
	}
	bool operator()(Seed const * const sptr, int vertex) const {
		return operator()(&(sptr->matrix()), vertex);
	}
	bool operator()(LabelledSeed const * const sptr, int vertex) const {
		return operator()(&(sptr->matrix()), vertex);
	}
	mutable std::map<int, std::vector<int>> _vertex_cache;
};
template <class M>
using  _EGGreen = __ExchangeGraph<M, _green_exchange::InfiniteTypeSink>;
}
typedef _green_exchange::_EGGreen<Seed> GreenExchangeGraph;
typedef _green_exchange::_EGGreen<LabelledSeed> GreenLabelledExchangeGraph;
typedef _green_exchange::_EGGreen<QuiverMatrix> GreenLabelledQuiverGraph;
typedef _green_exchange::_EGGreen<EquivQuiverMatrix> GreenQuiverGraph;
}

