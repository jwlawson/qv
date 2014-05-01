/*
 * oriented_cycle_info.cc
 */
#include "oriented_cycle_info.h"

#include "cycle_info.h"
#include <iostream>

namespace cluster {

	OrientedCycleInfo::OrientedCycleInfo(const QuiverMatrix& matrix)
		: matrix_() {
			std::cout << "Matrix:" << matrix << std::endl;
		CycleInfo inf(matrix);
		std::vector<int> vertices;
		int size = std::min(matrix.num_rows(), matrix.num_cols());
		vertices.reserve(size);
		std::cout <<"Vertices:" << std::endl;
		for(int i = 0; i < size; ++i) {
			if(inf.cycle_contains(i)){
				std::cout << i << " ";
				vertices.push_back(i);
			}
		}
		std::cout << std::endl;
		vertices.shrink_to_fit();
		if(vertices.size() > 0) {
			matrix_ = EquivQuiverMatrix(vertices.size(), vertices.size());
			matrix.submatrix(vertices, vertices, matrix_);
		}
		std::cout << matrix_ << std::endl;
	}

	bool OrientedCycleInfo::equals(const OrientedCycleInfo& cycle) const {
		return matrix_.equals(cycle.matrix_);
	}

	std::size_t OrientedCycleInfo::hash() const {
		return matrix_.hash();
	}
}


