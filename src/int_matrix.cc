#include "int_matrix.h"
#include <functional>
#include <sstream>
#include <iostream>

namespace cluster {

	IntMatrix::IntMatrix() : num_rows_(0), num_cols_(0), data_(0), hashcode_(113) {}

	IntMatrix::IntMatrix(const int rows, const int cols)
		: num_rows_(rows),
			num_cols_(cols),
			data_(rows *cols, 0),
			hashcode_(113) {}

	IntMatrix::IntMatrix(const int rows, const int cols, const int values[])
		: num_rows_(rows),
			num_cols_(cols),
			data_(rows *cols),
			hashcode_(0) {
		for (int i = 0; i < rows * cols; i++) {
			data_[i] = values[i];
		}
		hashcode_=compute_hash();
	}
	IntMatrix::IntMatrix(const IntMatrix &mat)
		: num_rows_(mat.num_rows_),
			num_cols_(mat.num_cols_),
			data_(mat.data_),
			hashcode_(mat.hashcode_) {
		}

	IntMatrix::IntMatrix(IntMatrix&& mat)
		: IntMatrix() {
		swap(*this, mat);
	}

	IntMatrix::IntMatrix(std::string str) {
		std::stringstream ss;
		ss << str;
		std::string buf;
		std::vector<int> data;
		data.reserve((str.size()/2) -4);
		int row_size = 0;
		int col_size = 0;
		bool start = false;
		bool end = false;
		bool in_row = false;
		bool row_done = false;
		while(!end) {
			ss >> buf;
			if(!start) {
				if("{"==buf) {
				start = true;
				}
				continue;
			}
			if("{"==buf){
				/* Started new row. */
				in_row = true;
			} else if("}"==buf){
				/* Reached end of row. */
				if(!in_row) {
					/* End of matrix */
					end = true;
				} else {
					row_done = true;
					in_row = false;
					++col_size;
				}
			} else {
				/* Should be integer. */
				if(!row_done) {
					++row_size;
				}
				data.push_back(std::stoi(buf));
			}
		}

		data.shrink_to_fit();

		num_rows_ = row_size;
		num_cols_ = col_size;
		data_ = std::move(data);
		hashcode_ = compute_hash();
	}


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
		return std::move(result);
	}

	const std::vector<int> IntMatrix::get_col(const int col) const {
		std::vector<int> result(num_rows_);
		int count = col;
		for (int j = 0; j < num_rows_; j++) {
			result[j] = data_[count];
			count += num_cols_;
		}
		return std::move(result);
	}

	void IntMatrix::set(IntMatrix mat) {
		swap(*this, mat);
	}

	void IntMatrix::set(const int row, const int col, const int value) {
		data_[get_index(row, col)] = value;
	}

	void IntMatrix::reset() {
		hashcode_ = compute_hash();
	}

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

	const int* IntMatrix::data() const {
		return data_.data();
	}


	bool IntMatrix::equals(const IntMatrix &rhs) const {
		return hashcode_ == rhs.hashcode_ && data_ == rhs.data_;
	}

	std::size_t IntMatrix::hash() const {
		return hashcode_;
	}

	bool IntMatrix::are_equal(const IntMatrix &lhs, const IntMatrix &rhs) {
		return lhs.hashcode_ == rhs.hashcode_ && lhs.data_ == rhs.data_;
	}


	void IntMatrix::submatrix(const int row, const int col, IntMatrix &result) const {
		int resInd = 0;
		int origInd = 0;

		while (resInd < result.num_rows_ * result.num_cols_) {
			bool changed;
			do {
				changed = false;
				if (origInd == row * num_cols_) {
					origInd += num_cols_;
					changed = true;
				}
				if (origInd % num_cols_ == col) {
					origInd++;
					changed = true;
				}
			} while (changed);
			result.data_[resInd++] = data_[origInd++];
		}
		result.reset();
	}

	void IntMatrix::enlarge_matrix(const int e, const int extra_cols, IntMatrix &result) const {
		int sub = 0;
		for (int index = 0; index < result.num_rows_ * result.num_cols_; index++) {
			if (index % (num_cols_ + extra_cols) >= num_cols_) {
				result.data_[index] = 0;
				sub++;
			} else if (index >= num_rows_ * (num_cols_ + extra_cols)) {
				result.data_[index] = 0;
			} else {
				result.data_[index] = data_[index - sub];
			}
		}
		result.reset();
	}
	/* Private methods */

	int IntMatrix::get_index(const int row, const int col) const {
		return row * num_cols_ + col;
	}

	std::size_t IntMatrix::compute_hash() const {
		std::size_t hash = 113;
		for (int i = 0; i < num_rows_ * num_cols_; i++) {
			hash *= 31;
			hash += data_[i];
		}
		return hash;
	}


	void IntMatrix::mult(const IntMatrix &left, const IntMatrix &right, IntMatrix &result) const {
		int col_inc = right.num_rows_;
		int leftInd;
		int leftIndStart = 0;
		int rightInd;
		int calcInd = 0;
		for (int i = 0; i < left.num_rows_; i++) {
			for (int j = 0; j < right.num_cols_; j++) {
				leftInd = leftIndStart;
				rightInd = j;
				result.data_[calcInd] = 0;
				while (leftInd < leftIndStart + left.num_cols_) {
					result.data_[calcInd] += left.data_[leftInd] * right.data_[rightInd];
					leftInd++;
					rightInd += col_inc;
				}

				calcInd++;
			}
			leftIndStart += left.num_cols_;
		}
		result.reset();
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
		swap(first.hashcode_, second.hashcode_);
	}
}


