/*
 * stream_iterator.cc
 */
#include "stream_iterator.h"

#include <iostream> /* Need iostream for std::cin. */

#include "equiv_quiver_matrix.h"

namespace cluster {
	
	template<class T>
	StreamIterator<T>::StreamIterator() 
		:	has_next_(false),
			str_(),
			input_(&std::cin, NullDeleter()) {}

	template<class T>
	StreamIterator<T>::StreamIterator(std::istream& stream)
		: input_(&stream, NullDeleter()) {
		has_next_ = std::getline(*input_, str_);
	}

	template<class T>
	std::shared_ptr<T> StreamIterator<T>::next() {
		std::shared_ptr<T> result = std::make_shared<T>(str_);
		do {
			has_next_ = std::getline(*input_, str_);
		} while (has_next_ && str_[0] != '{');
		return std::move(result);
	}

	template<class T>
	bool StreamIterator<T>::has_next() {
		return has_next_;
	}

	template class StreamIterator<IntMatrix>;
	template class StreamIterator<QuiverMatrix>;
	template class StreamIterator<EquivQuiverMatrix>;
}

