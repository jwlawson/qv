/*
 * link_holder.cc
 */

#include "link_holder.h"
#include "int_matrix.h"
#include "quiver_matrix.h"
#include "equiv_quiver_matrix.h"

namespace cluster {
	template<class T>
		LinkHolder<T>::LinkHolder() {}

	template<class T>
	LinkHolder<T>::LinkHolder(const int size)
		: links_(size, false) {}

	template<class T>
	LinkHolder<T>::~LinkHolder() {}

	template<class T>
		void LinkHolder<T>::size(const int size){
			links_.reserve(size);
		}
	template<class T>
	void LinkHolder<T>::matrix(T mat) {
		matrix_ = mat;
	}
	template<class T>
	T LinkHolder<T>::matrix() const {
		return matrix_;
	}
	template<class T>
	void LinkHolder<T>::link(const int k) {
		links_[k] = true;
	}
	template<class T>
	bool LinkHolder<T>::has_link(const int k) const {
		return links_[k];
	}
	template<class T>
	bool LinkHolder<T>::is_complete() const {
		for (uint i = 0; i < links_.size(); i++) {
			if (!links_[i]) {
				return false;
			}
		}
		return true;
	}

	template class LinkHolder<IntMatrix>;
	template class LinkHolder<QuiverMatrix>;
	template class LinkHolder<EquivQuiverMatrix>;
}

