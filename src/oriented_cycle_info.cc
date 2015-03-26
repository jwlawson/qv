/*
 * oriented_cycle_info.cc
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
#include "oriented_cycle_info.h"

#include "cycle_info.h"

namespace cluster {

	OrientedCycleInfo::OrientedCycleInfo(const QuiverMatrix& matrix)
		: matrix_() {
		CycleInfo inf(matrix);
		std::vector<int> vertices;
		int size = std::min(matrix.num_rows(), matrix.num_cols());
		vertices.reserve(size);
		for(int i = 0; i < size; ++i) {
			if(inf.cycle_contains(i)){
				vertices.push_back(i);
			}
		}
		vertices.shrink_to_fit();
		if(vertices.size() > 0) {
			matrix_ = EquivQuiverMatrix(vertices.size(), vertices.size());
			matrix.submatrix(vertices, vertices, matrix_);
		}
	}

	bool OrientedCycleInfo::equals(const OrientedCycleInfo& cycle) const {
		return matrix_.equals(cycle.matrix_);
	}

	std::size_t OrientedCycleInfo::hash() const {
		return matrix_.hash();
	}

	std::ostream& operator<<(std::ostream& os, const OrientedCycleInfo& cycle) {
		os << cycle.matrix_;
		return os;
	}
}


