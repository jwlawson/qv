/**
 * sized_submatrix_iterator.cc
 */
#include "sized_submatrix_iterator.h"

namespace cluster {
	SizedSubmatrixIterator::SizedSubmatrixIterator(int size, IntMatrix& matrix)
		: m_(matrix),
			size_(size) {
	}
	bool SizedSubmatrixIterator::has_next(){
		return false;
	}
	IntMatrix SizedSubmatrixIterator::next() {
		return IntMatrix();
	}
}

