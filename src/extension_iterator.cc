/*
 * extension_iterator.cc
 */

#include "extension_iterator.h"
#include "quiver_matrix.h"
#include "equiv_quiver_matrix.h"

namespace cluster {
	template<class T>
	ExtensionIterator<T>::ExtensionIterator(const T& matrix) 
			: matrix_(matrix.num_rows() + 1, matrix.num_cols() + 1),
				size_(matrix.num_rows()),
				index_(0),
				max_(ipow(5, size_)) {
		matrix.enlarge_matrix(1, 1, matrix_);
	}
		
	template<class T>
	T ExtensionIterator<T>::next() {
		T result(matrix_);
		int num(index_);
		for (int i = 0; i < size_; i++) {
			int val = (((int) (num / ipow(5, i))) % 5) - 2;
			result.set(size_, i, val);
			result.set(i, size_, -val);
		}
		++index_;
		result.reset();
		return std::move(result);
	}

	template<class T>
	bool ExtensionIterator<T>::has_next() {
		return index_ < max_;
	}

	template<class T>
	int ExtensionIterator<T>::ipow (int x, int p) {
		int i = 1;
		for (int j = 1; j <= p; j++)  i *= x;
		return i;
	}
	
	template class ExtensionIterator<QuiverMatrix>;
	template class ExtensionIterator<EquivQuiverMatrix>;
}

