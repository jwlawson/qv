/*
 * stream_submatrix_iterator.cc
 */
#include "stream_submatrix_iterator.h"

#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {

	template<class T>
	std::shared_ptr<T> StreamSubIter<T>::next() {
		if(!sub_iter_.has_next()) {
			std::shared_ptr<T> mat = stream_iter_.next();
			removed_ = -1;
			matrix_ = mat;
			sub_iter_ = SubmatrixIterator<T>(*mat);
		}
		++removed_;
		return sub_iter_.next();
	}

	template<class T>
	typename StreamSubIter<T>::MatrixSub StreamSubIter<T>::next_info() {
		MatrixSub result;
		result.submatrix = next();
		result.matrix = matrix_;
		result.removed = removed_;
		return result;
	}

	template<class T>
	bool StreamSubIter<T>::has_next() {
		return sub_iter_.has_next() || stream_iter_.has_next();
	}

	template class StreamSubIter<QuiverMatrix>;
	template class StreamSubIter<EquivQuiverMatrix>;
}

