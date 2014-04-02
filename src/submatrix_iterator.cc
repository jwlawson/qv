/*
 * submatrix_iterator.cc
 */

#include "submatrix_iterator.h"
#include "int_matrix.h"
#include "quiver_matrix.h"
#include "equiv_quiver_matrix.h"

namespace cluster {
	template<class T>
	SubmatrixIterator<T>::SubmatrixIterator(const T matrix)
		: matrix_(matrix), count_(0), max_(matrix.num_rows()) {}

	template<class T>
	T SubmatrixIterator<T>::next() {
		T result(matrix_.num_rows() - 1, matrix_.num_cols() - 1);
		matrix_.submatrix(count_, count_, result);
		count_++;
		return result;
	}

	template<class T>
	bool SubmatrixIterator<T>::has_next() {
		return count_ >= max_;
	}


	template class SubmatrixIterator<IntMatrix>;
	template class SubmatrixIterator<QuiverMatrix>;
	template class SubmatrixIterator<EquivQuiverMatrix>;

}

