/**
 * sized_submatrix_iterator.h
 *
 * Iterator which goes over all square submatrices of a given matrix which are a
 * specified size, attained by removing the same rows and columns.
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
		/** Get the rows which make up the previously returned submatrix. */
		std::vector<int> get_rows();

		private:
		/** Initial matrix to take submatrices of. */
		IntMatrix m_;
		/** Next matrix to return from iterator. */
		IntMatrix next_;
		/** Size of all submatrices to return. */
		size_t size_;
		/** Size of smallest dimension of m_ */
		int max_val_;
		/** Vector of which rows and columns were in the last returned submatrix. */
		std::vector<int> last_rows_;
		/** Vector of which rows and columns to include in the next submatrix. */
		std::vector<int> rows_;
		/** True if the iterator will return a valid matrix next. */
		bool has_next_;

		/** Compute the next set of rows and columns for the next submatrix. */
		void check_add_index(size_t ind);
	};
}

