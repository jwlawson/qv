/*
 * stream_iterator.cc
 */
#include "stream_iterator.h"

#include <iostream>

#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {
	
	template<class T>
	StreamIterator<T>::StreamIterator() 
		:	has_next_(false),
			input_(std::cin){}

	template<class T>
	StreamIterator<T>::StreamIterator(std::istream& stream) 
		: input_(stream) {
		has_next_ = std::getline(input_, str_);
	}

	template<class T>
	std::shared_ptr<T> StreamIterator<T>::next() {
		std::shared_ptr<T> result = std::make_shared<T>(str_);
		do {
			has_next_ = std::getline(input_, str_);
		} while (has_next_ && str_[0] != '{');
		return std::move(result);
	}

	template<class T>
	bool StreamIterator<T>::has_next() {
		return has_next_;
	}

	template class StreamIterator<QuiverMatrix>;
	template class StreamIterator<EquivQuiverMatrix>;
}

