/*
 * equivalence_checker.cc
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
#include "equivalence_checker.h"

#include <numeric>

namespace cluster {

	std::shared_ptr<EquivalenceChecker>
	EquivalenceChecker::Get(const int size) {
		static std::map<int, std::shared_ptr<EquivalenceChecker>> instance_map{};
		auto position = instance_map.find(size);
		if(position == instance_map.end()) {
			auto inserted = instance_map.emplace(size,
					std::make_shared<EquivalenceChecker>(size));
			position = inserted.first;
		}
		return position->second;
	}

	EquivalenceChecker::EquivalenceChecker()
		:
			size_(0),
			maps_(0),
			last_row_map_(size_) {}

	EquivalenceChecker::EquivalenceChecker(const int size)
		:
			size_(size),
			maps_(size),
			last_row_map_(size_) {}

	bool
	EquivalenceChecker::are_equivalent(const M& a, const M& b) {
		if(a.num_rows() != b.num_rows() || a.num_cols() != b.num_cols() ) {
			return false;
		}
		/* Zero size check needs to come *after* the row and column checks,
		 * otherwise we could be comparing a matrix with non-zero size to one of
		 * zero size using a non-zero equivalence checker. */
		if(size_ == 0) {
			return true;
		}
		if (IntMatrix::are_equal(a, b)) {
			std::iota(last_row_map_.begin(), last_row_map_.end(), 0);
			return true;
		}
		if (!do_rows_match(a, b)) {
			return false;
		}
		bool result = check_perm(last_row_map_, 0, a, b);
		return result;
	}
	EquivalenceChecker::PermVecPtr
	EquivalenceChecker::valid_row_maps(const M& lhs, const M& rhs){
		PermVecPtr result = std::make_shared<std::vector<Permutation>>();
		bool rows_match = do_rows_match(lhs, rhs);
		if (!rows_match) {
			return result;
		}
		all_perms(last_row_map_, 0, lhs, rhs, result);
		return result;
	}

	/* Private methods */
	bool
	EquivalenceChecker::do_rows_match(const M& a, const M& b) {
		maps_.reset();
		bool rows_match = true;
		auto b_start_rows = b.rows_.begin();
		auto b_end_rows = b.rows_.end();
		int const * const a_data = a.data();
		int const * const b_data = b.data();
		for (int a_ind = 0; rows_match && a_ind < size_; a_ind++) {
			bool equiv = false;
			int const* a_row = a_data + a_ind * size_;
			// Go through each row of B which matches the row of A at index a_ind.
			auto next_row = std::find(b_start_rows, b_end_rows, a.rows_[a_ind]);
			while(next_row != b_end_rows) {
				// std::find returns an iterator, but we want the row index
				int index = std::distance(b_start_rows, next_row);
				int const* b_row = b_data + index * size_;
				if (std::is_permutation(a_row, a_row + size_, b_row)) {
					equiv = true;
					maps_.update_row_mapping(a_ind, index);
				}
				++next_row;
				next_row = std::find(next_row, b_end_rows, a.rows_[a_ind]);
			}
			if (!equiv) {
				rows_match = false;
			}
		}
		return rows_match;
	}
	bool
	EquivalenceChecker::check_perm(Permutation & row_map, int index,
			const M& a, const M& b) {
		bool result = false;
		if(index == size_) {
		/* Row_map contains a complete permutation, that is each index
		 * represents a row, and the value at that index shows where to permute
		 * that row.
		 *
		 * We have a full permutation which maps a to b, so the matrices should be
		 * equivalent. We have been checking that the permutation is valid as we
		 * construct it, so shouldn't need to explicitly compute the permted
		 * matrices.
		 */
			result = true;
		} else {
			/* Row_map is not a complete permutation. Recursively add another entry to
			 * the vector.
			 *
			 * Want to check each possible permutation, so consider each possible
			 * mapping, and for each fill in the remaining row_map entries. */
			for(size_t i = 0; i < maps_.row_mappings[index].size() && !result; ++i) {
				int val = maps_.row_mappings[index][i];
				/* Check that val is not already in the permutation */
				bool not_already_used = true;
				for(int j = 0; not_already_used && j < index; ++j) {
					not_already_used = !(row_map[j] == val);
				}
				if(not_already_used) {
					/* Check that adding the perm index -> val would give a good
					 * permutation, using what has been constructed so far. */
					bool skip = false;
					int const * a_data = a.data() + index * size_;
					int const * b_row_data = b.data() + val * size_;
					for(int j = 0; !skip && j < index; ++j, ++a_data) {
						skip = ( *a_data != b_row_data[row_map[j]] );
					}
					if(!skip) {
						row_map[index] = val;
						result = check_perm(row_map, index + 1, a, b);
					}
				}
			}
		}
		return result;
	}
	void
	EquivalenceChecker::all_perms(Permutation & row_map, int index,
			const M& a, const M& b, PermVecPtr perms) {
		if(index == size_) {
			perms->push_back(row_map);
		} else {
			for(size_t i = 0; i < maps_.row_mappings[index].size(); ++i) {
				int val = maps_.row_mappings[index][i];
				bool not_already_used = true;
				for(int j = 0; not_already_used && j < index; ++j) {
					not_already_used = !(row_map[j] == val);
				}
				if(not_already_used) {
					/* Check that adding the perm index -> val would give a good
					 * permutation, using what has been constructed so far. */
					bool skip = false;
					int const * a_data = a.data() + index * size_;
					int const * b_row_data = b.data() + val * size_;
					for(int j = 0; !skip && j < index; ++j, ++a_data) {
						skip = ( *a_data != b_row_data[row_map[j]] );
					}
					if(!skip) {
						row_map[index] = val;
						all_perms(row_map, index + 1, a, b, perms);
					}
				}
			}
		}
	}
}

