/*
 * equiv_underlying_graph.cc
 */

#include "equiv_underlying_graph.h"
#include <cstdlib>

namespace cluster {

	EquivUnderlyingGraph::EquivUnderlyingGraph(const IntMatrix& mat) 
		: EquivQuiverMatrix(mat.num_rows(), mat.num_cols()) {
		
			for(int i = 0; i < mat.num_rows() * mat.num_cols(); ++i) {
				data_[i] = std::abs(mat.data()[i]);
			}
			reset();
		}

}

