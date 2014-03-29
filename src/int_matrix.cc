#include "int_matrix.h"

namespace cluster{

	IntMatrix::IntMatrix(const int rows, const int cols) : num_rows_(rows), num_cols_(cols)
	{
		data_ = new int[rows * cols];
		for (int i = 0; i < rows * cols; i++) {
			data_[i] = 0;
		}
	}

	IntMatrix::IntMatrix(const int rows, const int cols, const int values[]) 
		: num_rows_(rows), num_cols_(cols)
	{
		data_ = new int[rows * cols];
		for (int i = 0; i < rows * cols; i++) {
			data_[i] = values[i];
		}
	}
	IntMatrix::IntMatrix(const IntMatrix& mat) : IntMatrix(mat.num_rows_, mat.num_cols_, mat.data_){}

	IntMatrix::~IntMatrix()
	{
		delete[] data_;
	}

	/* Public methods */

	const int IntMatrix::num_rows() const
	{
		return num_rows_;
	}

	const int IntMatrix::num_cols() const
	{
		return num_cols_;
	}

	const int IntMatrix::get(const int row, const int col) const
	{
		return data_[get_index(row, col)];
	}

	const std::vector<int> IntMatrix::get_row(const int row) const
	{
		std::vector<int> result (num_cols_);
		int count = row * num_cols_;
		for (int j = 0; j < num_cols_; j++) {
			result[j] = data_[count++];
		}
		return result;
	}

	const std::vector<int> IntMatrix::get_col(const int col) const
	{
		std::vector<int> result(num_rows_);
		int count = col;
		for (int j = 0; j < num_rows_; j++) {
			result[j] = data_[count];
			count += num_cols_;
		}
		return result;
	}

	void IntMatrix::set(const IntMatrix mat)
	{
		IntMatrix::set(mat.num_rows_, mat.num_cols_, mat.data_);
	}

	void IntMatrix::set(const int rows, const int cols, const int *values)
	{
		reset();
		data_ = new int[rows * cols];
		num_rows_ = rows;
		num_cols_ = cols;
		for (int i = 0; i < rows * cols; i++) {
			data_[i] = values[i];
		}
	}

	void IntMatrix::set(const int row, const int col, const int value)
	{
		data_[get_index(row, col)] = value;
	}

	std::unique_ptr<IntMatrix> IntMatrix::copy() const
	{
		std::unique_ptr<IntMatrix> result(new IntMatrix(num_rows_, num_cols_));
		for (int i = 0; i < num_rows_ * num_cols_; i++) {
			result->data_[i] = data_[i];
		}
		return result;
	}

	void IntMatrix::reset()
	{
		hashcode_ = 0;
	}

	const int IntMatrix::zero_row() const
	{
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

	const bool IntMatrix::equals(const IntMatrix& rhs) const
	{
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

	const std::size_t IntMatrix::hash()
	{
		int hash = hashcode_;
		if (hash == 0) {
			hash = compute_hash();
			hashcode_ = hash;
		}
		return hashcode_;
	}

	const bool IntMatrix::are_equal(const IntMatrix& lhs, const IntMatrix& rhs)
	{
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

	const int IntMatrix::get_index(const int row, const int col) const
	{
		return row * num_cols_ + col;
	}

	const std::size_t IntMatrix::compute_hash() const
	{
		std::size_t hash = 113;
		for (int i = 0; i < num_rows_ * num_cols_; i++) {
			hash *= 523;
			hash += data_[i];
		}
		return hash;
	}
	
	std::ostream& operator<<(std::ostream& os, const IntMatrix& mat)
	{
		os << "{ ";
		for(int i = 0; i < mat.num_rows_; i++){
			os << "{ ";
			for(int j = 0; j < mat.num_cols_; j++){
				os << mat.data_[i*mat.num_cols_ + j] << " ";
			}
			os << "} ";
		}
		os << "}";
		return os;
	}
}

