/*
 * stream_iterator.cc
 * Copyright 2014-2015 John Lawson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
		has_next_ = static_cast<bool>(std::getline(*input_, str_));
	}

	template<class T>
	std::shared_ptr<T> StreamIterator<T>::next() {
		std::shared_ptr<T> result = std::make_shared<T>(str_);
		do {
			has_next_ = static_cast<bool>(std::getline(*input_, str_));
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

