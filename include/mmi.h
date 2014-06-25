/*
 * mmi.h
 *
 * Contains method to check whether a matrix is minimally mutation infinite.
 */
#pragma once

#include "quiver_matrix.h"
#include "equiv_quiver_matrix.h"

namespace cluster {
	namespace mmi {
		/**
		 * Check whether the matrix is minimally mutation infinite.
		 *
		 * First the matrix is checked to be infinite, then all submatrixes are
		 * checked to be finite.
		 *
		 * This uses the fast method which cannot say for sure whether a matrix is
		 * finite, so the results should be checked with the slower mmi check.
		 * @param matrix Matrix to check
		 * @return true if the matrix is probably minimally mutation infinite
		 */
		bool fast_mmi(const QuiverMatrix& matrix);

		/**
		 * Add matrices which are known to be finite to the finite checker.
		 * @param mat Mutation finite matrix to add
		 */
		void add_finite(const std::shared_ptr<EquivQuiverMatrix>& mat);

		/**
		 * Check whether the matrix is minimally mutation infinite.
		 *
		 * This will check using the fast method and then fall back to checking the
		 * whole mutation class to ensure that any matrix which is *probably*
		 * mutation finite actually is.
		 *
		 * Finding the size of the mutation class can be very slow, especially for
		 * large matrices, so this method will be significantly slower than the
		 * fast_mmi method, however this will give definite results.
		 *
		 * @param matrix The matrix to check
		 * @return true if the matrix is minimally mutation infinite
		 */
		bool slow_mmi(const QuiverMatrix& matrix);
	}
}

