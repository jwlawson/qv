/*
 * underlying_graph.cc
 */
#include "underlying_graph.h"

#include <cstdlib>

namespace cluster {

	UnderlyingGraph::UnderlyingGraph(const IntMatrix& mat) 
		: IntMatrix(mat.num_rows(), mat.num_cols()) {
		
			for(int i = 0; i < mat.num_rows() * mat.num_cols(); ++i) {
				data_[i] = std::abs(mat.data()[i]);
			}
			reset();
		}

}

