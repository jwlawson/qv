/*
 * equivalence_checker.cc
 */
#include "equivalence_checker.h"

#include <algorithm>

#include "array_utils.h"

namespace cluster {
	std::weak_ptr<EquivalenceChecker> EquivalenceChecker::instance_ = 
		std::make_shared<EquivalenceChecker>();

	std::shared_ptr<EquivalenceChecker> EquivalenceChecker::Get(const int size) {
		std::shared_ptr<EquivalenceChecker> result = instance_.lock();
		if (!result || result->size_ != size) {
			result.reset(new EquivalenceChecker(size));
			instance_ = result;
		}
		return result;
	}

	EquivalenceChecker::EquivalenceChecker()
		: ap_(0, 0), 
			pb_(0, 0), 
			size_(0),
			maps_(0) {}

	EquivalenceChecker::EquivalenceChecker(const int size)
		: ap_(size, size),
		  pb_(size, size),
			size_(size),
			maps_(size) {}

	bool EquivalenceChecker::are_equivalent(const M& a, const M& b) {
		if(a.num_rows() != b.num_rows() ) {
			return false;
		}
		if(a.num_cols() != b.num_cols() ) {
			return false;
		}
		if(size_ == 0) {
			return true;
		}
		if (IntMatrix::are_equal(a, b)) {
			return true;
		}
		
		if (!sums_equivalent(a, b)) {
			return false;
		}

		maps_.reset();

		bool rows_match = do_rows_match(a, b);
		if (!rows_match) {
			return false;
		}

		bool cols_match = do_columns_match(a, b);
		if (!cols_match) {
			return false;
		}
		using s_t  = std::size_t;
		bool valid;
		for(int i = 0; i < size_; ++i) {
			valid = false;
			for(s_t j = 0; j < maps_.row_mappings[i].size() && !valid; ++j) {
				int val = maps_.row_mappings[i][j];
				for(s_t k = 0; k < maps_.col_mappings[val].size() && !valid; ++k) {
					if(maps_.col_mappings[val][k] == i) {
						valid = true;
					}
				}
			}
			if(!valid) {
				return false;
			}
		}

		std::vector<int> vec;
		vec.reserve(size_);
		return check_perm(vec, 0, a, b);

	}

	EquivalenceChecker& EquivalenceChecker::operator=(EquivalenceChecker mat) {
		swap(*this, mat);
		return *this;
	}

	/* Private methods */

	bool EquivalenceChecker::do_columns_match(const M& a, const M& b) {
		bool cols_match = true;
		for (int a_ind = 0; a_ind < size_ && cols_match; a_ind++) {
			int inCol = std::count(b.cols_.begin(), b.cols_.end(), a.cols_[a_ind]);
			int index = -1;
			std::vector<int> a_col_val = a.get_col(a_ind);
			bool equiv = false;
			for (int i = 0; i < inCol; i++) {
				index = arrays::next_index_of(b.cols_, a.cols_[a_ind], index);
				std::vector<int> b_col_val = b.get_col(index);
				if (arrays_equivalent(a_col_val, b_col_val)) {
					equiv = true;
					maps_.update_column_mapping(index, a_ind);
				}
			}
			if (!equiv) {
				cols_match = false;
			}
		}
		return cols_match;
	}
	bool EquivalenceChecker::do_rows_match(const M& a, const M& b) {
		bool rows_match = true;
		for (int a_ind = 0; a_ind < size_ && rows_match; a_ind++) {
			int inRow = std::count(b.rows_.begin(), b.rows_.end(), a.rows_[a_ind]);
			int index = -1;
			std::vector<int> a_row_vals = a.get_row(a_ind);
			bool equiv = false;
			for (int i = 0; i < inRow; i++) {
				index = arrays::next_index_of(b.rows_, a.rows_[a_ind], index);
				std::vector<int> b_row_vals = b.get_row(index);
				if (arrays_equivalent(a_row_vals, b_row_vals)) {
					equiv = true;
					maps_.update_row_mapping(a_ind, index);
				}
			}
			if (!equiv) {
				rows_match = false;
			}
		}
		return rows_match;
	}

	bool EquivalenceChecker::sums_equivalent(const M& a, const M& b) const {
		return std::is_permutation(a.rows_.begin(), a.rows_.end(), b.rows_.begin())
			&& std::is_permutation(a.cols_.begin(), a.cols_.end(), b.cols_.begin());
	}

	bool EquivalenceChecker::arrays_equivalent(const std::vector<int>& a,
	    const std::vector<int>& b) const {
		return std::is_permutation(a.begin(), a.end(), b.begin());
	}

	bool EquivalenceChecker::check_perm(std::vector<int>& row_map, int index,
			const M& a, const M& b) {
		bool result = false;
		if(index == size_) {
			/* Compute. */
			std::vector<int> col_map(size_);
			for(int i = 0; i < size_; i++) {
				col_map[row_map[i]] = i;
			}
			a.permute_rows(row_map, ap_);
			b.permute_columns(col_map, pb_);
			result = ap_.equals(pb_);

		} else {
			for(std::size_t i = 0; i < maps_.row_mappings[index].size() && !result; ++i) {
				int val = maps_.row_mappings[index][i];
				bool rec = true;
				for(std::size_t j = 0; j < row_map.size(); ++j) {
					if(row_map[j] == val) {
						rec = false;
					}
				}
				if(rec) {
					row_map[index] = val;
					result = check_perm(row_map, index + 1, a, b);
				}
			}
		}
		return result;
	}

	/* Friends. */

	void swap(EquivalenceChecker& f, EquivalenceChecker& s) {
		using std::swap;
		swap(f.ap_, s.ap_);
		swap(f.pb_, s.pb_);
		swap(f.size_, s.size_);
		swap(f.maps_, s.maps_);
	}
}

