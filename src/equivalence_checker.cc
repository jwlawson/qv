/*
 * equivalence_checker.cc
 */
#include "equivalence_checker.h"

#include <algorithm>

#include "array_utils.h"

namespace cluster {
	namespace {
		typedef EquivQuiverMatrix M;
		typedef std::vector<int> Permutation;
		typedef std::shared_ptr<std::vector<Permutation>> PermVecPtr;
	}

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
			maps_(0),
			last_row_map_(size_),
			last_col_map_(size_),
			a_row_vals_(size_),
			b_row_vals_(size_){}

	EquivalenceChecker::EquivalenceChecker(const int size)
		: ap_(size, size),
		  pb_(size, size),
			size_(size),
			maps_(size),
			last_row_map_(size_),
			last_col_map_(size_),
			a_row_vals_(size_),
			b_row_vals_(size_){}

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
			for(size_t i = 0; i < last_row_map_.size(); i++) {
				last_row_map_[i] = i;
			}
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
		bool result = check_perm(last_row_map_, 0, a, b);
		return result;
	}
	PermVecPtr EquivalenceChecker::valid_row_maps(const M& lhs, const M& rhs){
		PermVecPtr result = std::make_shared<std::vector<Permutation>>();
		maps_.reset();
		bool rows_match = do_rows_match(lhs, rhs);
		if (!rows_match) {
			return result;
		}
		all_perms(last_row_map_, 0, lhs, rhs, result);
		return result;
	}
	EquivalenceChecker& EquivalenceChecker::operator=(EquivalenceChecker mat) {
		swap(*this, mat);
		return *this;
	}

	/* Private methods */

	bool EquivalenceChecker::do_rows_match(const M& a, const M& b) {
		bool rows_match = true;
		for (int a_ind = 0; a_ind < size_ && rows_match; a_ind++) {
			int inRow = std::count(b.rows_.begin(), b.rows_.end(), a.rows_[a_ind]);
			int index = -1;
			a.get_row(a_ind, a_row_vals_);
			bool equiv = false;
			for (int i = 0; i < inRow; i++) {
				index = arrays::next_index_of(b.rows_, a.rows_[a_ind], index);
				b.get_row(index, b_row_vals_);
				if (arrays_equivalent(a_row_vals_, b_row_vals_)) {
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
			/* Compute. Row_map contains a complete permutation, that is each index
			 * represents a row, and the value at that index shows where to permute
			 * that row.
			 *
			 * So actually perform the permutation and check for equality. */
			for(int i = 0; i < size_; i++) {
				last_col_map_[row_map[i]] = i;
			}
			a.permute_rows(row_map, ap_);
			b.permute_columns(last_col_map_, pb_);
			result = ap_.equals(pb_);

		} else {
			/* Row_map is not a complete permutation. Recursively add another entry to
			 * the vector.
			 *
			 * Want to check each possible permutation, so consider each possible
			 * mapping, and for each fill in the remaining row_map entries. */
			for(size_t i = 0; i < maps_.row_mappings[index].size() && !result; ++i) {
				int val = maps_.row_mappings[index][i];
				bool not_already_used = true;
				for(int j = 0; j < index; ++j) {
					if(row_map[j] == val) {
						not_already_used = false;
					}
				}
				if(not_already_used) {
					row_map[index] = val;
					result = check_perm(row_map, index + 1, a, b);
				}
			}
		}
		return result;
	}
	void EquivalenceChecker::all_perms(Permutation& row_map, int index,
			const M& a, const M& b, PermVecPtr perms) {
		if(index == size_) {
			/* Compute. Row_map contains a complete permutation, that is each index
			 * represents a row, and the value at that index shows where to permute
			 * that row.
			 *
			 * So actually perform the permutation and check for equality. */
			for(int i = 0; i < size_; i++) {
				last_col_map_[row_map[i]] = i;
			}
			a.permute_rows(row_map, ap_);
			b.permute_columns(last_col_map_, pb_);
			if(ap_.equals(pb_)) {
				perms->push_back(row_map);
			}
		} else {
			/* Row_map is not a complete permutation. Recursively add another entry to
			 * the vector.
			 *
			 * Want to check each possible permutation, so consider each possible
			 * mapping, and for each fill in the remaining row_map entries. */
			for(size_t i = 0; i < maps_.row_mappings[index].size(); ++i) {
				int val = maps_.row_mappings[index][i];
				bool not_already_used = true;
				for(int j = 0; j < index; ++j) {
					if(row_map[j] == val) {
						not_already_used = false;
					}
				}
				if(not_already_used) {
					row_map[index] = val;
					all_perms(row_map, index + 1, a, b, perms);
				}
			}
		}
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

