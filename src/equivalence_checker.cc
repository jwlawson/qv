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
		: permutations_(), 
			ap_(0, 0), 
			pb_(0, 0), 
			size_(0) {}
	EquivalenceChecker::EquivalenceChecker(const int size)
		: permutations_(factorial(size), IntMatrix(size, size)),
			ap_(size, size),
		  pb_(size, size),
			size_(size) {
		if(size == 0) {
			/* Don't even bother trying to do anything. */
			return;
		}
		int count = 0;
		int *values = new int[size];
		for (int i = 0; i < std::pow(size, size); i++) {
			bool valid = perm_values(size, i, values);
			if (!valid) {
				continue;
			}
			for (int j = 0; j < size; j++) {
				permutations_[count].set(j, values[j], 1);
			}
			permutations_[count].reset();
			count++;
		}
		delete [] values;
	}
	EquivalenceChecker::~EquivalenceChecker() {}
	bool EquivalenceChecker::are_equivalent(const IntMatrix &a,
	    const IntMatrix &b) {
		if (IntMatrix::are_equal(a, b)) {
			return true;
		}
		int a_rows = a.num_rows();
		int a_cols = a.num_cols();
		std::vector<int> a_row_sum(a_rows);
		std::vector<int> a_col_sum(a_cols);
		std::vector<int> a_abs_row_sum(a_rows);
		std::vector<int> a_abs_col_sum(a_cols);

		std::vector<int> b_row_sum(a_rows);
		std::vector<int> b_col_sum(a_cols);
		std::vector<int> b_abs_row_sum(a_rows);
		std::vector<int> b_abs_col_sum(a_cols);
		for (int i = 0; i < a_rows; i++) {
			for (int j = 0; j < a_cols; j++) {
				int aVal = a.get(i, j);
				a_row_sum[i] += aVal;
				a_col_sum[j] += aVal;
				a_abs_row_sum[i] += abs(aVal);
				a_abs_col_sum[j] += abs(aVal);

				int bVal = b.get(i, j);
				b_row_sum[i] += bVal;
				b_col_sum[j] += bVal;
				b_abs_row_sum[i] += abs(bVal);
				b_abs_col_sum[j] += abs(bVal);
			}
		}
		if (!arrays_equivalent(a_row_sum, b_row_sum)
		    || !arrays_equivalent(a_col_sum, b_col_sum)
		    || !arrays_equivalent(a_abs_row_sum, b_abs_row_sum)
		    || !arrays_equivalent(a_abs_col_sum, b_abs_col_sum)) {
			return false;
		}

		std::vector<int> row_mappings = new_mapping_array(a_rows);
		std::vector<int> col_mappings = new_mapping_array(a_cols);

		bool rows_match =
		  do_rows_match(a, b, a_rows, a_row_sum, a_abs_row_sum, b_row_sum, b_abs_row_sum,
		                row_mappings);
		if (!rows_match) {
			return false;
		}

		bool cols_match =
		  do_columns_match(a, b, a_cols, a_col_sum, a_abs_col_sum, b_col_sum,
		                   b_abs_col_sum,
		                   col_mappings);
		if (!cols_match) {
			return false;
		}
		for(uint i = 0; i < permutations_.size(); ++i) {
			if (!permutation_valid(a_rows, a_cols, col_mappings, permutations_[i])) {
				continue;
			}
			// Check if PA == BP or PAP^(-1) == B
			a.mult_right(permutations_[i], ap_);
			b.mult_left(permutations_[i], pb_);
			if (IntMatrix::are_equal(pb_, ap_)) {
				return true;
			}
		}
		return false;
	}

	EquivalenceChecker& EquivalenceChecker::operator=(EquivalenceChecker mat) {
		swap(*this, mat);
		return *this;
	}

	/* Private methods */

	bool EquivalenceChecker::perm_values(const int size, const int i,
	                                     int *values) const {
		int id = i;
		for (int j = 0; j < size; j++) {
			values[j] = id % (size);
			for (int k = 0; k < j; k++) {
				if (values[j] == values[k]) {
					return false;
				}
			}
			id /= size;
		}
		return true;
	}
	int EquivalenceChecker::factorial(const int num) const {
		if (num == 1) {
			return 1;
		}
		if (num == 0) {
			return 0;
		}
		return num * factorial(num - 1);
	}
	std::vector<int> EquivalenceChecker::new_mapping_array(const int size) const {
		return std::vector<int> (size, 57);
	}
	bool EquivalenceChecker::do_columns_match(const IntMatrix& a, const IntMatrix& b,
	    const int a_cols,
	    const std::vector<int>& a_col_sum,
			const std::vector<int>& a_abs_col_sum,
	    const std::vector<int>& b_col_sum,
	    const std::vector<int>& b_abs_col_sum,
			std::vector<int> &col_mappings) const {
		bool cols_match = true;
		for (int a_ind = 0; a_ind < a_cols; a_ind++) {
			int inCol = arrays::number_in(b_col_sum, a_col_sum[a_ind]);
			int index = -1;
			std::vector<int> a_col_val = a.get_col(a_ind);
			bool equiv = false;
			for (int i = 0; i < inCol; i++) {
				index = arrays::next_index_of(b_col_sum, a_col_sum[a_ind], index);
				if (b_abs_col_sum[index] != a_abs_col_sum[a_ind]) {
					// Columns will only be equivalent if they have the same absolute
					// value sum
					continue;
				}
				std::vector<int> b_col_val = b.get_col(index);
				if (arrays_equivalent(a_col_val, b_col_val)) {
					equiv = true;
					update_mapping(a_cols, col_mappings, index, a_ind);
				}
			}
			if (!equiv) {
				cols_match = false;
			}
		}
		return cols_match;
	}
	bool EquivalenceChecker::do_rows_match(const IntMatrix& a, const IntMatrix& b,
	    const int a_rows,
	    const std::vector<int>& a_row_sum, const std::vector<int>& a_abs_row_sum,
	    const std::vector<int>& b_row_sum,
	    const std::vector<int>& b_abs_row_sum, std::vector<int> &row_mappings) const {
		bool rows_match = true;
		for (int a_ind = 0; a_ind < a_rows && rows_match; a_ind++) {
			int inRow = arrays::number_in(b_row_sum, a_row_sum[a_ind]);
			int index = -1;
			std::vector<int> a_row_vals = a.get_row(a_ind);
			bool equiv = false;
			for (int i = 0; i < inRow; i++) {
				index = arrays::next_index_of(b_row_sum, a_row_sum[a_ind], index);
				if (b_abs_row_sum[index] != a_abs_row_sum[a_ind]) {
					// Rows will only be equivalent if they have the same absolute value sum
					continue;
				}
				std::vector<int> b_row_vals = b.get_row(index);
				if (arrays_equivalent(a_row_vals, b_row_vals)) {
					equiv = true;
					update_mapping(a_rows, row_mappings, a_ind, index);
				}
			}
			if (!equiv) {
				rows_match = false;
			}
		}
		return rows_match;
	}
	void EquivalenceChecker::update_mapping(const int numRows,
			std::vector<int> &mappings, const int aIndex,
			const int bIndex) const {
		mappings[bIndex] *= numRows;
		mappings[bIndex] += aIndex;
	}
	bool EquivalenceChecker::permutation_valid(const int a_rows, const int a_cols,
	    const std::vector<int>& col_mappings,
	    const IntMatrix& p) const {
		for (uint i = 0; i < col_mappings.size(); i++) {
			if (!perm_column_valid(a_cols, col_mappings, p, i)) {
				return false;
			}
		}
		return true;
	}
	bool EquivalenceChecker::perm_column_valid(const int num,
	    std::vector<int> mappings, const IntMatrix& perm,
	    const int index)const {
		while (mappings[index] != 57) {
			if (perm.get(index, mappings[index] % num) == 1) {
				return true;
			}
			mappings[index] /= num;
		}
		return false;
	}
	bool EquivalenceChecker::arrays_equivalent(std::vector<int> a,
	    std::vector<int> b) const {
		std::sort(a.begin(), a.end());
		std::sort(b.begin(), b.end());
		bool equal = arrays::equal(a, b);
		return equal;
	}

	void swap(EquivalenceChecker& f, EquivalenceChecker& s) {
		using std::swap;
		swap(f.permutations_, s.permutations_);
		swap(f.ap_, s.ap_);
		swap(f.pb_, s.pb_);
		swap(f.size_, s.size_);
	}
}

