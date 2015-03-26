/*
 * equiv_underlying_graph.cc
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
#include "equiv_underlying_graph.h"
#include <cstdlib>
#include <algorithm>

namespace cluster {
	namespace {
		typedef std::vector<std::pair<int, int>> PairVec;
	}
	EquivUnderlyingGraph::EquivUnderlyingGraph(const IntMatrix& mat) 
		: EquivQuiverMatrix(mat.num_rows(), mat.num_cols()) {
		
			for(int i = 0; i < mat.num_rows() * mat.num_cols(); ++i) {
				data_[i] = std::abs(mat.data()[i]);
			}
			reset();
		}
	void EquivUnderlyingGraph::set_matrix(const IntMatrix& mat) {
		if(mat.num_rows() != num_rows_) {
			rows_ = PairVec(mat.num_rows(), std::make_pair(0,0));
		}
		if(mat.num_cols() != num_cols_) {
			cols_ = PairVec(mat.num_cols(), std::make_pair(0,0));
		}
		if(mat.num_rows() == num_rows_ && mat.num_cols() == num_cols_) {
			/* Don't need to allocate a new array */
			for(int i = 0; i < num_rows_ * num_cols_; i++) {
				data_[i] = std::abs(mat.data()[i]);
			}
		} else {
			data_ = IntVector(mat.vector());
			std::for_each(data_.begin(), data_.end(), [](int& a)->void{a = a < 0 ? -a : a;});
			num_rows_ = mat.num_rows();
			num_cols_ = mat.num_cols();
		}
		reset();
	}

}

