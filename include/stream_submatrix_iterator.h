/*
 * stream_submatrix_iterator.h
 */
#pragma once

#include "stream_iterator.h"
#include "submatrix_iterator.h"

namespace cluster {
	template<class T>
	class StreamSubIter {

		public:
			StreamSubIter() : stream_iter_() {}
			StreamSubIter(std::istream& stream) : stream_iter_(stream) {}

			std::shared_ptr<T> next();
			bool has_next();

		private:
			StreamIterator<T> stream_iter_;
			SubmatrixIterator<T> sub_iter_;

	};
}

