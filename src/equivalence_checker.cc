#include "equivalence_checker.h"
#include "array_utils.h"
#include <algorithm>
#include <iostream>

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
			ai_(0),
			bi_(0),
			maps_(0) {}

	EquivalenceChecker::EquivalenceChecker(const int size)
		: ap_(size, size),
		  pb_(size, size),
			size_(size),
			ai_(size),
			bi_(size),
			maps_(size) {}

	bool EquivalenceChecker::are_equivalent(const IntMatrix &a,
	    const IntMatrix &b) {
		if (IntMatrix::are_equal(a, b)) {
			return true;
		}
		
		calc_sums(a, b);

		if (!sums_equivalent()) {
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
		bool valid;
		for(int i = 0; i < size_; ++i) {
			valid = false;
			for(uint j = 0; j < maps_.row_mappings[i].size() && !valid; ++j) {
				int val = maps_.row_mappings[i][j];
				for(uint k = 0; k < maps_.col_mappings[val].size() && !valid; ++k) {
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

	int EquivalenceChecker::factorial(const int num) const {
		if (num == 1) {
			return 1;
		}
		if (num == 0) {
			return 0;
		}
		return num * factorial(num - 1);
	}

	void EquivalenceChecker::calc_sums(const IntMatrix &a, const IntMatrix &b) {
		ai_.reset();
		bi_.reset();
		for (int i = 0; i < size_; i++) {
			for (int j = 0; j < size_; j++) {
				int aVal = a.get(i, j);
				ai_.row_sum[i] += aVal;
				ai_.col_sum[j] += aVal;
				ai_.abs_row_sum[i] += abs(aVal);
				ai_.abs_col_sum[j] += abs(aVal);

				int bVal = b.get(i, j);
				bi_.row_sum[i] += bVal;
				bi_.col_sum[j] += bVal;
				bi_.abs_row_sum[i] += abs(bVal);
				bi_.abs_col_sum[j] += abs(bVal);
			}
		}
	}

	bool EquivalenceChecker::do_columns_match(const IntMatrix& a, const IntMatrix& b) {
		bool cols_match = true;
		for (int a_ind = 0; a_ind < size_ && cols_match; a_ind++) {
			int inCol = arrays::number_in(bi_.col_sum, ai_.col_sum[a_ind]);
			int index = -1;
			std::vector<int> a_col_val = a.get_col(a_ind);
			bool equiv = false;
			for (int i = 0; i < inCol; i++) {
				index = arrays::next_index_of(bi_.col_sum, ai_.col_sum[a_ind], index);
				if (bi_.abs_col_sum[index] != ai_.abs_col_sum[a_ind]) {
					// Columns will only be equivalent if they have the same absolute
					// value sum
					continue;
				}
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
	bool EquivalenceChecker::do_rows_match(const IntMatrix& a, const IntMatrix& b) {
		bool rows_match = true;
		for (int a_ind = 0; a_ind < size_ && rows_match; a_ind++) {
			int inRow = arrays::number_in(bi_.row_sum, ai_.row_sum[a_ind]);
			int index = -1;
			std::vector<int> a_row_vals = a.get_row(a_ind);
			bool equiv = false;
			for (int i = 0; i < inRow; i++) {
				index = arrays::next_index_of(bi_.row_sum, ai_.row_sum[a_ind], index);
				if (bi_.abs_row_sum[index] != ai_.abs_row_sum[a_ind]) {
					// Rows will only be equivalent if they have the same absolute value sum
					continue;
				}
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

	bool EquivalenceChecker::sums_equivalent() const {
		return arrays_equivalent(ai_.row_sum, bi_.row_sum)
		    && arrays_equivalent(ai_.col_sum, bi_.col_sum)
		    && arrays_equivalent(ai_.abs_row_sum, bi_.abs_row_sum)
		    && arrays_equivalent(ai_.abs_col_sum, bi_.abs_col_sum);
	}

	bool EquivalenceChecker::arrays_equivalent(std::vector<int> a,
	    std::vector<int> b) const {
		std::sort(a.begin(), a.end());
		std::sort(b.begin(), b.end());
		bool equal = arrays::equal(a, b);
		return equal;
	}

	bool EquivalenceChecker::check_perm(std::vector<int>& row_map, int index,
			const IntMatrix& a, const IntMatrix& b) {
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
			for(uint i = 0; i < maps_.row_mappings[index].size() && !result; ++i) {
				int val = maps_.row_mappings[index][i];
				bool rec = true;
				for(uint j = 0; j < row_map.size(); ++j) {
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
	}
}

