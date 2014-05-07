/*
 * oriented_cycle_info.cc
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
}


