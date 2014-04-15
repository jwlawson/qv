/*
 * submatrix_iterator.cc
 */
#include "submatrix_iterator.h"

#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {


	template<class T>
	SubmatrixIterator<T>::SubmatrixIterator() {}

	template<class T>
	SubmatrixIterator<T>::SubmatrixIterator(const T& matrix)
		: matrix_(matrix), count_(0), max_(matrix.num_rows()) {}

	template<class T>
	std::shared_ptr<T> SubmatrixIterator<T>::next() {
		std::shared_ptr<T> result = std::make_shared<T>(matrix_.num_rows() - 1,
				matrix_.num_cols() - 1);
		matrix_.subquiver(count_, *result);
		++count_;
		return result;
	}

	template<class T>
	bool SubmatrixIterator<T>::has_next() {
		return count_ < max_;
	}


	template class SubmatrixIterator<QuiverMatrix>;
	template class SubmatrixIterator<EquivQuiverMatrix>;

}

