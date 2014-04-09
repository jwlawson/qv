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
			size_(0),
			ai_(0),
			bi_(0),
			maps_(0) {}

	EquivalenceChecker::EquivalenceChecker(const int size)
		: permutations_(factorial(size), IntMatrix(size, size)),
			ap_(size, size),
		  pb_(size, size),
			size_(size),
			ai_(size),
			bi_(size),
			maps_(size) {
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

	bool EquivalenceChecker::are_equivalent(const IntMatrix &a,
	    const IntMatrix &b) {
		//std::cout << "Checking matrices" << std::endl;
		//std::cout << a << std::endl << b << std::endl;
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

		//std::cout << "Rows: ";
		for(int i = 0; i < size_; i++) {
			//std::cout << i<< "->";
			for(uint j = 0; j < maps_.row_mappings[i].size(); j++) {
				//std::cout << maps_.row_mappings[i][j] << " ";
			}
			//std::cout << ", ";
		}
		//std::cout << std::endl << "Cols: ";
		for(int i = 0; i < size_; i++) {
			//std::cout << i << "->";
			for(uint j = 0; j < maps_.col_mappings[i].size(); j++){
				//std::cout << maps_.col_mappings[i][j] << " ";
			}
			//std::cout << ", ";
		}
		//std::cout << std::endl;

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

		for(uint i = 0; i < permutations_.size(); ++i) {
			if (!permutation_valid(permutations_[i])) {
				continue;
			}
			// Check if PA == BP or PAP^(-1) == B
			a.mult_right(permutations_[i], ap_);
			b.mult_left(permutations_[i], pb_);
			if (IntMatrix::are_equal(pb_, ap_)) {
			//std::cout << "Equal" << permutations_[i] << std::endl;
				return true;
			}
		}
		//std::cout << "Not equal" << std::endl;
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
	bool EquivalenceChecker::permutation_valid(
	    const IntMatrix& p) const {
		//std::cout << "Checking " << p << std::endl;
		for (uint i = 0; i < maps_.col_mappings.size(); i++) {
			if(
					!perm_column_valid(p, i)
				|| 
				!perm_row_valid(p, i)
				) {
		//std::cout <<"Not valid" << std::endl;
				return false;
			}
		}
		//std::cout << "Valid"<< std::endl;
		return true;
	}
	bool EquivalenceChecker::perm_column_valid(
	    const IntMatrix& perm,
	    const int index)const {
		//std::cout << "c" << index << "-> ";
		for(uint i = 0; i < maps_.col_mappings[index].size(); ++i) {
			//std::cout << maps_.col_mappings[index][i] << " (" << perm.get(maps_.col_mappings[index][i], index) << ") ";
			if (perm.get(maps_.col_mappings[index][i], index) == 1) {
				return true;
			}
		}
		return false;
	}

	bool EquivalenceChecker::perm_row_valid(
			const IntMatrix& perm,
			const int index) const {
		//std::cout << "r" << index << "-> ";
		for(uint i = 0; i < maps_.row_mappings[index].size(); ++i) {
			//std::cout << maps_.row_mappings[index][i] << " (" << perm.get(index, maps_.row_mappings[index][i]) << ") ";
			if(perm.get(index, maps_.row_mappings[index][i]) == 1) {
				return true;
			}
		}
		return false;
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

	void swap(EquivalenceChecker& f, EquivalenceChecker& s) {
		using std::swap;
		swap(f.permutations_, s.permutations_);
		swap(f.ap_, s.ap_);
		swap(f.pb_, s.pb_);
		swap(f.size_, s.size_);
	}
}

