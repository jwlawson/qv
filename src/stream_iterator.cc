/*
 * stream_iterator.cc
 */
#include "stream_iterator.h"

#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {
	
	template<class T>
	StreamIterator<T>::StreamIterator(std::istream& stream) 
		: input_(stream) {
		has_next_ = std::getline(input_, str_);
	}

	template<class T>
	std::shared_ptr<T> StreamIterator<T>::next() {
		std::shared_ptr<T> result = std::make_shared<T>(str_);
		has_next_ = std::getline(input_, str_);
		return result;
	}

	template<class T>
	bool StreamIterator<T>::has_next() {
		return has_next_;
	}

	template class StreamIterator<QuiverMatrix>;
	template class StreamIterator<EquivQuiverMatrix>;
}

