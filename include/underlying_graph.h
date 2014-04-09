/*
 * underlying_graph.h
 *
 * Contains the UnderlyingGraph class.
 *
 * Underlying graph provides an easy way to construct the matrix oft he
 * underlying graph from a quiver.
 */

#pragma once
#include "int_matrix.h"

namespace cluster {
	class UnderlyingGraph : public IntMatrix {

		public:
			UnderlyingGraph(const IntMatrix&);

	};
}

