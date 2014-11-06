/**
 * sized_submatrix_iterator.h
 *
 * Iterator which goes over all square submatrices of a given matrix which are a
 * specified size.
 *
 * This iterator does not do any checks on the submatrices nor does it ensure
 * that there are no zero rows.
 */
#pragma once

#include "int_matrix.h"

namespace cluster {

	class SizedSubmatrixIterator {
		public:
		/**
		 * Create an iterator which returns submatrices of size `size` of the
		 * provided matrix.
		 * @param size Size of each submatrix.
		 * @param matrix Initial matrix to take submatrices of.
		 */
		SizedSubmatrixIterator(int size, IntMatrix& matrix);
		/**
		 * Check if the iterator will return a valid submatrix if next is called.
		 * @return true if next() will return a valid submatrix.
		 */
		bool has_next();
		/**
		 * Get the next submatrix.
		 */
		IntMatrix next();

		private:
		IntMatrix m_;
		IntMatrix next_;
		int size_;
		std::vector<int> rows_;
	};
}

