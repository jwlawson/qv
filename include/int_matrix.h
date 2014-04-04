#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include <iostream>

namespace cluster {
	class IntMatrix {
	 public:
		IntMatrix();
		IntMatrix(const int rows, const int cols);
		IntMatrix(const int rows, const int cols, const int values[]);
//	 private:
		IntMatrix(const IntMatrix &mat);
//	 public:
		IntMatrix(IntMatrix &&mat);
	 private:
		IntMatrix &operator=(IntMatrix mat);
	 public:
		int num_rows() const;
		int num_cols() const;
		int get(const int row, const int col) const;
		const std::vector<int> get_row(const int row) const;
		const std::vector<int> get_col(const int col) const;
		void set(const IntMatrix mat);
		void set(const int row, const int col, const int value);
		void reset();
		int zero_row() const;
		virtual bool equals(const IntMatrix &mat) const;
		std::size_t hash() const;
		static bool are_equal(const IntMatrix &lhs, const IntMatrix &rhs);
		friend std::ostream &operator<< (std::ostream &os, const IntMatrix &mat);
		friend void swap(IntMatrix &first, IntMatrix &second);

		template<class T>
		void mult_left(const IntMatrix &left, T &result) const {
			IntMatrix::mult(left, *this, result);
		}
		template<class T>
		void mult_right(const IntMatrix &right, T &result) const {
			IntMatrix::mult(*this, right, result);
		}
		template<class T>
		void submatrix(const int row, const int col, T &result) const {
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
		// TODO Remove unneeded parameters.
		template<class T>
		void enlarge_matrix(const int e, const int extra_cols, T &result) const {
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

	 protected:
		int num_rows_;
		int num_cols_;
		std::vector<int> data_;

	 private:
		std::size_t hashcode_;

		int get_index(const int row, const int col) const;
		virtual std::size_t compute_hash() const;

		template<class T>
		void mult(const IntMatrix &left, const IntMatrix &right, T &result) const {
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
	};
}
namespace std {
	/* Add hash function to the std::hash struct. */
	template <>
	struct hash<cluster::IntMatrix> {
		size_t operator()(const cluster::IntMatrix &x) const {
			return x.hash();
		}
	};
	/* Add equals function to std::equal_to */
	template<>
	struct equal_to<cluster::IntMatrix> {
		bool operator()(const cluster::IntMatrix &lhs,
		                const cluster::IntMatrix &rhs) const {
			return lhs.equals(rhs);
		}
	};
}


