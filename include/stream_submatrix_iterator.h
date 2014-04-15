/*
 * stream_submatrix_iterator.h
 */
#pragma once

#include "stream_iterator.h"
#include "submatrix_iterator.h"

namespace cluster {
	template<class T>
	class StreamSubIter {

		private:
			typedef std::shared_ptr<T> MatrixPtr;
		public:
			StreamSubIter() : stream_iter_() {}
			StreamSubIter(std::istream& stream) : stream_iter_(stream) {}

			struct MatrixSub{
				MatrixPtr matrix;
				MatrixPtr submatrix;
				int removed;
			};

			std::shared_ptr<T> next();
			MatrixSub next_info();
			bool has_next();

		private:
			StreamIterator<T> stream_iter_;
			SubmatrixIterator<T> sub_iter_;
			MatrixPtr matrix_;
			int removed_;
	};
}

