/*
 * skew_symmetric_iterator.h
 *
 * Headers for the SkewSymmetricIterator class.
 *
 * The iterator goes over all matrices of a certain size retruning all possible
 * matrices with values between -2 and 2.
 */
#pragma once

#include <memory>

#include "equiv_quiver_matrix.h"

namespace cluster {
	class SkewSymmetricIterator {

		private:
			typedef EquivQuiverMatrix M;
			typedef typename std::shared_ptr<M> MPtr;

		public:
			/** 
			 * Create a new iterator which provides all skew-symmetri matrices of the
			 * specified size.
			 * @param size Size of matrices to return
			 */
			SkewSymmetricIterator(const int size);
			/**
			 * Check whether there is a next matrix to take from the iterator.
			 * @return true if next() will return a valid matrix
			 */
			bool has_next() const;
			/**
			 * Get the next matrix from the iterator.
			 * @return next matrix
			 */
			MPtr next();

		private:
			/** Matrix holding the next to return. */
			IntMatrix matrix_;
			/** Index that the col index cannot exceed. */
			int col_comp_;
			/** Current index of which matrix will be returned next. */
			long index_;
			/** Number of variable entries in the matrix which need to be set. */
			int num_vars_;
			/** Max value that index_ can take. */
			long max_;

			/** Calculate x^p. */
			long int ipow(const int x, const int p) const;
			/** Calculate the number of variables in the matrix. */
			int num_vars(int size) const;

	};
}

