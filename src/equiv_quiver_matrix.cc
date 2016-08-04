/*
 * equiv_quiver_matrix.cc
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
#include "equiv_quiver_matrix.h"

#include <algorithm>
#include <boost/functional/hash.hpp>

#include "array_utils.h"

namespace cluster {
	EquivQuiverMatrix::EquivQuiverMatrix()
		: QuiverMatrix()
		, rows_()
		, checker_(EquivalenceChecker::Get(0))
		, sorted_rows_()
	{}

	EquivQuiverMatrix::EquivQuiverMatrix(const int rows, const int cols)
		: QuiverMatrix(rows, cols)
		, rows_(rows, std::make_pair(0,0))
		, checker_(EquivalenceChecker::Get(rows))
		, sorted_rows_(rows_)
	{}

	EquivQuiverMatrix::EquivQuiverMatrix(const int rows, const int cols,
			const int values[])
		: QuiverMatrix(rows, cols, values)
		, rows_(rows, std::make_pair(0,0))
		, checker_(EquivalenceChecker::Get(rows))
		, sorted_rows_(rows_)
	{
		reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const IntMatrix& matrix)
		: QuiverMatrix(matrix)
		, rows_(matrix.num_rows(), std::make_pair(0,0))
		, checker_(EquivalenceChecker::Get(matrix.num_rows()))
		, sorted_rows_(rows_)
	{
		reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(std::string const& str)
		: QuiverMatrix(str)
		, rows_(num_rows(), std::make_pair(0,0))
		, checker_(EquivalenceChecker::Get(num_rows_))
		, sorted_rows_(rows_)
	{
		reset();
	}

	void EquivQuiverMatrix::set_matrix(const IntMatrix& mat) {
		if(mat.num_rows() != num_rows_) {
			rows_ = PairVector(mat.num_rows(), std::make_pair(0,0));
			sorted_rows_ = rows_;
		}
		IntMatrix::set_matrix(mat);
		reset();
	}

	/* Note that the static_cast will create a copy of mat */
	bool EquivQuiverMatrix::equals(const IntMatrix &mat) const {
		auto e_mat = static_cast<EquivQuiverMatrix>(mat);
		return checker_->are_equivalent(*this, e_mat);
	}

	bool EquivQuiverMatrix::equals(const EquivQuiverMatrix &mat) const {
		return checker_->are_equivalent(*this, mat);
	}

	void EquivQuiverMatrix::reset() {
		for(size_t i = 0; i < rows_.size(); ++i) {
			rows_[i].first = 0;
			rows_[i].second = 0;
		}
		int row_i = 0;
		int col_i = 0;
		int i = 0;
		while(i < num_rows_ * num_cols_) {
			rows_[row_i].first += data_[i];
			rows_[row_i].second += std::abs(data_[i]);

			i++;
			col_i++;
			if(col_i == num_cols_) {
				col_i = 0;
				row_i++;
			}
		}
		IntMatrix::reset();
	}

	EquivQuiverMatrix::CPermutation
	EquivQuiverMatrix::get_permutation(const IntMatrix& mat) const {
		this->equals(mat);
		return checker_->last_row_map();
	}

	EquivQuiverMatrix::PermVecPtr
	EquivQuiverMatrix::all_permutations(const EquivQuiverMatrix& mat) const {
		return checker_->valid_row_maps(*this, mat);
	}

	/* Private methods */

	std::size_t EquivQuiverMatrix::compute_hash() const {
		std::size_t hash = 137;
		sorted_rows_ = rows_;
		std::sort(sorted_rows_.begin(), sorted_rows_.end());
		boost::hash_combine(hash, arrays::hash(sorted_rows_));

		return hash;
	}
}
