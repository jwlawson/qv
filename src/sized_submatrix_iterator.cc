/**
 * sized_submatrix_iterator.cc
 */
#include "sized_submatrix_iterator.h"

namespace cluster {
	SizedSubmatrixIterator::SizedSubmatrixIterator(int size, IntMatrix& matrix)
		: m_(matrix),
			next_(size, size),
			size_(size),
			max_val_(std::min(matrix.num_rows(), matrix.num_cols()) - 1),
			last_rows_(size),
			rows_(size),
			has_next_(true) {
		for(size_t i = 0; i < rows_.size(); i++){
			rows_[i] = i;
		}
	}
	bool SizedSubmatrixIterator::has_next(){
		return has_next_;
	}
	IntMatrix SizedSubmatrixIterator::next() {
		m_.submatrix(rows_, rows_, next_);
		last_rows_.assign(rows_.begin(), rows_.end());
		check_add_index(0);
		return next_;
	}
	std::vector<int> SizedSubmatrixIterator::get_rows() {
		return last_rows_;
	}
	void SizedSubmatrixIterator::check_add_index(size_t ind) {
		if(rows_[ind] == max_val_) {
			has_next_ = false;
			return;
		}
		if(ind != rows_.size() - 1 && rows_[ind] + 1 == rows_[ind + 1]) {
			rows_[ind] = ind;
			return check_add_index(ind + 1);
		} else {
			rows_[ind] += 1;
		}
	}
		
}

