/*
 * mutation_class.cc
 */

#include "quiver_matrix.h"
#include "equiv_quiver_matrix.h"
#include "mutation_class.h"

namespace cluster {

	template< class T>
	const T MutationClass<T>::INFINITE;

	template<class T>
	MutationClass<T>::MutationClass() {}

	template<class T>
	MutationClass<T>::MutationClass(T mat)
		: matrix_(mat),
		  should_calc_(true),
		  size_(mat.num_rows()),
		  map_(),
		  queue_() {}

	template<class T>
	T MutationClass<T>::next() {
		T result = queue_.front();
		if (compute_mutations(result)) {
			return result;
		} else {
			return INFINITE;
		}
	}

	template<class T>
	bool MutationClass<T>::has_next() {
		return !queue_.empty();
	}

	template<class T>
	void MutationClass<T>::calc_more(const bool calc) {
		should_calc_ = calc;
	}

	template<class T>
	bool MutationClass<T>::compute_mutations(const T &mat) {
		for (int i = 0; i < size_ && should_calc_; i++) {
			if (mutate_at(mat, i)) {
				T new_matrix(mat.num_rows(), mat.num_cols());
				new_matrix = mat.mutate(i, new_matrix);
				if (have_seen(new_matrix)) {
					seen_matrix(new_matrix, mat, i);
					if (is_complete(new_matrix)) {
						remove_complete(new_matrix);
					}
				} else {
					if (new_matrix.is_infinite()) {
						return false;
					}
					unseen_matrix(new_matrix, mat, i);
				}
			}
		}
		return true;
	}

	template<class T>
	bool MutationClass<T>::is_complete(const T &matrix) {
		return map_[matrix].is_complete();
	}

	template<class T>
	void MutationClass<T>::remove_handled(const T &matrix) {
		map_.erase(matrix);
	}

	template<class T>
	void MutationClass<T>::remove_complete(const T &matrix) {
		map_.erase(matrix);
	}

	template<class T>
	void MutationClass<T>::seen_matrix(const T &matrix, const T &previous,
	                                   const int vertex) {
		map_[matrix].link(vertex);
		map_[previous].link(vertex);
	}

	template<class T>
	void MutationClass<T>::unseen_matrix(const T &matrix, const T &previous,
	                                     const int vertex) {
		queue_.push_back(matrix);
		map_[matrix].link(vertex);
		map_[previous].link(vertex);
	}

	template<class T>
	bool MutationClass<T>::have_seen(const T &matrix) {
		return map_.count(matrix) > 0;
	}

	template<class T>
	bool MutationClass<T>::mutate_at(const T &matrix, const int i) {
		return map_[matrix].has_link(i);
	}
	template class MutationClass<QuiverMatrix>;
	template class MutationClass<EquivQuiverMatrix>;
}

