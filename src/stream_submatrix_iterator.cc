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
			sub_iter_ = SubmatrixIterator<T>(*(stream_iter_.next()));
		}
		return sub_iter_.next();
	}

	template<class T>
	bool StreamSubIter<T>::has_next() {
		return sub_iter_.has_next() || stream_iter_.has_next();
	}

	template class StreamSubIter<QuiverMatrix>;
	template class StreamSubIter<EquivQuiverMatrix>;
}

