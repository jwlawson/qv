/*
 * stream_submatrix_iterator.h
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
/**
 * Combines StreamIterator and SubmatrixIterator to read matrices from an input
 * stream and return all possible submatrices of these.
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

			T const& next();
			MatrixSub next_info();
			void next_info(MatrixSub& info);
			bool has_next();

		private:
			StreamIterator<T> stream_iter_;
			SubmatrixIterator<T> sub_iter_;
			MatrixPtr matrix_;
			int removed_;
	};
	template<class T>
	inline
	typename StreamSubIter<T>::MatrixSub
	StreamSubIter<T>::next_info() {
		MatrixSub result {nullptr, nullptr, 0};
		next_info(result);
		return result;
	}
}

