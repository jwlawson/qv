/*
 * mutation_class_size.h
 *
 * Contains a method to find the size of the mutation class of a matrix.
 */
#pragma once

#include "quiver_matrix.h"

namespace cluster {
	namespace classsize {
		/**
		 * Find the size of the mutation class of the matrix.
		 * @param matrix Matrix to check
		 * @return The size of the mutation class
		 */
		int Size(const QuiverMatrix& matrix);

	}
}

