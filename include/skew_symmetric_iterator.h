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
			SkewSymmetricIterator(const int size);

			bool has_next() const;
			MPtr next();

		private:
			IntMatrix matrix_;
			int col_comp_;
			int index_;
			int num_vars_;
			int max_;

			int ipow(int x, int p) const;
			int num_vars(int size) const;

	};
}

