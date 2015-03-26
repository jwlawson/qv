/*
 * submatrix_iterator.cc
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
#include "submatrix_iterator.h"

#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {


	template<class T>
	SubmatrixIterator<T>::SubmatrixIterator() 
		: count_(0), max_(0) {}

	template<class T>
	SubmatrixIterator<T>::SubmatrixIterator(const T& matrix)
		: matrix_(matrix), count_(0), max_(matrix.num_rows()) {}

	template<class T>
	std::shared_ptr<T> SubmatrixIterator<T>::next() {
		std::shared_ptr<T> result = std::make_shared<T>(matrix_.num_rows() - 1,
				matrix_.num_cols() - 1);
		matrix_.subquiver(count_, *result);
		++count_;
		return std::move(result);
	}

	template<class T>
	bool SubmatrixIterator<T>::has_next() {
		return count_ < max_;
	}


	template class SubmatrixIterator<QuiverMatrix>;
	template class SubmatrixIterator<EquivQuiverMatrix>;

}

