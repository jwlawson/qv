/*
 * stream_submatrix_iterator.cc
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
#include "stream_submatrix_iterator.h"

#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {

	template<class T>
	std::shared_ptr<T> StreamSubIter<T>::next() {
		if(!sub_iter_.has_next()) {
			std::shared_ptr<T> mat = stream_iter_.next();
			removed_ = -1;
			matrix_ = mat;
			sub_iter_ = SubmatrixIterator<T>(*mat);
		}
		++removed_;
		return std::move(sub_iter_.next());
	}

	template<class T>
	typename StreamSubIter<T>::MatrixSub StreamSubIter<T>::next_info() {
		MatrixSub result;
		result.submatrix = next();
		result.matrix = matrix_;
		result.removed = removed_;
		return std::move(result);
	}

	template<class T>
	bool StreamSubIter<T>::has_next() {
		return sub_iter_.has_next() || stream_iter_.has_next();
	}

	template class StreamSubIter<QuiverMatrix>;
	template class StreamSubIter<EquivQuiverMatrix>;
}

