#include "int_matrix.h"
#include <functional>
#include <iostream>

namespace cluster {

	IntMatrix::IntMatrix() : num_rows_(0), num_cols_(0), data_(0) {}

	IntMatrix::IntMatrix(const int rows, const int cols)
		: num_rows_(rows), num_cols_(cols), data_(rows *cols, 0) {}

	IntMatrix::IntMatrix(const int rows, const int cols, const int values[])
		: num_rows_(rows), num_cols_(cols), data_(rows *cols) {
		for (int i = 0; i < rows * cols; i++) {
			data_[i] = values[i];
		}
	}
	IntMatrix::IntMatrix(const IntMatrix &mat)
		: num_rows_(mat.num_rows_), num_cols_(mat.num_cols_), data_(mat.data_) {}

	IntMatrix::IntMatrix(IntMatrix&& mat)
		: IntMatrix() {
		swap(*this, mat);
	}

	IntMatrix::~IntMatrix() {}

	/* Public methods */

	IntMatrix &IntMatrix::operator= (IntMatrix mat) {
		swap(*this, mat);
		return *this;
	}
	int IntMatrix::num_rows() const {
		return num_rows_;
	}

	int IntMatrix::num_cols() const {
		return num_cols_;
	}

	int IntMatrix::get(const int row, const int col) const {
		return data_[get_index(row, col)];
	}

	const std::vector<int> IntMatrix::get_row(const int row) const {
		std::vector<int> result(num_cols_);
		int count = row * num_cols_;
		for (int j = 0; j < num_cols_; j++) {
			result[j] = data_[count++];
		}
		return result;
	}

	const std::vector<int> IntMatrix::get_col(const int col) const {
		std::vector<int> result(num_rows_);
		int count = col;
		for (int j = 0; j < num_rows_; j++) {
			result[j] = data_[count];
			count += num_cols_;
		}
		return result;
	}

	void IntMatrix::set(IntMatrix mat) {
		swap(*this, mat);
	}

	void IntMatrix::set(const int row, const int col, const int value) {
		data_[get_index(row, col)] = value;
	}

	std::unique_ptr<IntMatrix> IntMatrix::copy() const {
		std::unique_ptr<IntMatrix> result(new IntMatrix(num_rows_, num_cols_));
		for (int i = 0; i < num_rows_ * num_cols_; i++) {
			result->data_[i] = data_[i];
		}
		return result;
	}

	void IntMatrix::reset() {}

	int IntMatrix::zero_row() const {
		bool isZero = false;
		int row = -1;
		for (int ind = 0; ind < num_rows_ * num_cols_; ind++) {
			if (ind % num_cols_ == 0) {
				if (isZero) {
					return row;
				}
				row++;
				isZero = true;
			}
			if (data_[ind] != 0) {
				isZero = false;
			}
		}
		if (isZero) {
			return row;
		}
		return -1;
	}

	bool IntMatrix::equals(const IntMatrix &rhs) const {
		if (num_rows_ * num_cols_ != rhs.num_rows_ * rhs.num_cols_) {
			return false;
		}
		for (int i = 0; i < num_rows_ * num_cols_; i++) {
			if (data_[i] != rhs.data_[i]) {
				return false;
			}
		}
		return true;
	}

	std::size_t IntMatrix::hash() const {
		return compute_hash();
	}

	bool IntMatrix::are_equal(const IntMatrix &lhs, const IntMatrix &rhs) {
		if (lhs.num_rows_ * lhs.num_cols_ != rhs.num_rows_ * rhs.num_cols_) {
			return false;
		}
		for (int i = 0; i < lhs.num_rows_ * lhs.num_cols_; i++) {
			if (lhs.data_[i] != rhs.data_[i]) {
				return false;
			}
		}
		return true;
	}

	/* Private methods */

	int IntMatrix::get_index(const int row, const int col) const {
		return row * num_cols_ + col;
	}

	std::size_t IntMatrix::compute_hash() const {
		std::size_t hash = 113;
		for (int i = 0; i < num_rows_ * num_cols_; i++) {
			hash *= 523;
			hash += data_[i];
		}
		return hash;
	}

	/* Friends */

	std::ostream &operator<< (std::ostream &os, const IntMatrix &mat) {
		os << "{ ";
		for (int i = 0; i < mat.num_rows_; i++) {
			os << "{ ";
			for (int j = 0; j < mat.num_cols_; j++) {
				os << mat.data_[i * mat.num_cols_ + j] << " ";
			}
			os << "} ";
		}
		os << "}";
		return os;
	}

	void swap(IntMatrix &first, IntMatrix &second) {
		using std::swap;
		swap(first.data_, second.data_);
		swap(first.num_rows_, second.num_rows_);
		swap(first.num_cols_, second.num_cols_);
	}
}


