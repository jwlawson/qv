/*
 * int_matrix.h
 *
 * Contains the IntMatrix class.
 *
 * An IntMatrix is a matrix whose entries can only be integers. The class
 * provides methods to multiply matrices together as well as taking submatrices
 * and enlarging matrices.
 */
#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include <ostream>
#include <string>

namespace cluster {
	class IntMatrix {
	 public:
		 /**
			* Create a default IntMatrix. The default matrix is essentially empty and
			* has size 0.
			*/
		IntMatrix();
		/**
		 * Create a new IntMatrix of the specified size. All values in the matrix
		 * will be set to zero.
		 *
		 * @param rows Number of rows
		 * @param cols Number of columns
		 */
		IntMatrix(const int rows, const int cols);
		/**
		 * Create an IntMatrix with specific values stored in the matrix.
		 *
		 * The values provided should be in a row major format, so all values for
		 * the first row followed by the second row and so on.
		 *
		 * @param rows Number of rows
		 * @param cols Number of columns
		 * @param values Array of values to put into the matrix
		 */
		IntMatrix(const int rows, const int cols, const int values[]);
		/**
		 * Copy constructor.
		 * @param mat Matrix to copy
		 */
		IntMatrix(const IntMatrix &mat);
		/**
		 * Move constructor. Leaves the passed matrix as a default IntMatrix.
		 * @param mat Matrix to move into this
		 */
		IntMatrix(IntMatrix &&mat);
		/**
		 * Construct an IntMatrix from a string. the string is expected to be
		 * formatted like one used in the << method.
		 *
		 * The correct formatting has spaces around each entry of the matrix and
		 * curly braces around each row and around the whole matrix.
		 *
		 * 		eg. 0 1 2
		 * 				3 4 5
		 * 				6 7 8
		 *
		 * 				{ { 0 1 2 } { 3 4 5 } { 6 7 8 } }
		 *
		 * @param str String containing matrix
		 */
		IntMatrix(std::string str);
		/**
		 * Assignment operator. Sets this matrix to the one passed.
		 * @param mat Matrix to copy
		 */
		IntMatrix &operator=(IntMatrix mat);
		/** 
		 * Get the number of rows in the matrix.
		 * @return Number of rows
		 */
		int num_rows() const;
		/**
		 * Get the number of columns in the matrix.
		 * @return Number fo columns
		 */
		int num_cols() const;
		/**
		 * Get the value stored in the matrix at a specific position.
		 * @param row Row position
		 * @param col Column position
		 * @return Value at (row, col)
		 */
		int get(const int row, const int col) const;
		/**
		 * Get a vector containing all entries in the specified row.
		 * @param row Row to return
		 * @return Vector of entries in the row
		 */
		const std::vector<int> get_row(const int row) const;
		/** 
		 * Get a vector containing all entries in the specified column.
		 * @param col Column to return
		 * @return Vector of entries in the column
		 */
		const std::vector<int> get_col(const int col) const;
		/**
		 * Set this matrix to be a copy of the matrix provided.
		 * @param mat Matrix to copy
		 */
		void set(const IntMatrix mat);
		/**
		 * Set the value in the matrix at the specified position to be the value
		 * provided.
		 *
		 * @param row Row position
		 * @param col Column position
		 * @param value Value to set
		 */
		void set(const int row, const int col, const int value);
		void reset();
		/**
		 * Find the first row that is all zeros. The value returned will then be
		 * the index of this row.
		 *
		 * If no such row exists then -1 is returned.
		 *
		 * @return Index of first all zero row or -1 if no such row exists.
		 */
		int zero_row() const;
		/**
		 * Check whether this matrix is equal to the one provided.
		 * @param mat Matrix to check
		 * @return true if the matries are equal
		 */
		virtual bool equals(const IntMatrix &mat) const;
		/**
		 * Get a pointer to the underlying array.
		 *
		 * Only use this if you are sure you know what you are doing.
		 * @return Pointer to the array that the matrix data is stored in
		 */
		const int* data() const;
		/**
		 * Compute the hash of the matrix.
		 *
		 * @return Hash of the matrix
		 */
		std::size_t hash() const;
		/**
		 * Static method to check whether two matrices are equal.
		 *
		 * This can be useful for checking two subclasses of IntMatrix are equal as
		 * IntMatrices if their own class has overriden the equals method.
		 * @param lhs First matrix to check
		 * @param rhs Second matrix to check
		 * @return true if the matrices are equal as IntMatrices
		 */
		static bool are_equal(const IntMatrix &lhs, const IntMatrix &rhs);
		/**
		 * Provide method to easily write IntMatrix objects to output stream.
		 */
		friend std::ostream &operator<< (std::ostream &os, const IntMatrix &mat);
		/**
		 * Swap the data in two IntMatrix objects.
		 * @param first First matrix
		 * @param second Second matrix
		 */
		friend void swap(IntMatrix &first, IntMatrix &second);

		/**
		 * Multiply this matrix on the left by the matrix provided. The result is
		 * written into the provided result matrix.
		 *
		 * 	ie result = this * left
		 *
		 * @param left Matrix to multiply by
		 * @param result Matrix to store the result
		 */
		template<class T>
		void mult_left(const IntMatrix &left, T &result) const {
			IntMatrix::mult(left, *this, result);
		}
		/**
		 * Multiply this matrix on the right by the matrix provided. The result is
		 * written into the provided result matrix.
		 *
		 * 	ie result = right * this
		 *
		 * @param right Matrix to multiply by
		 * @param result Matrix to store the result
		 */
		template<class T>
		void mult_right(const IntMatrix &right, T &result) const {
			IntMatrix::mult(*this, right, result);
		}
		/**
		 * Take the submatrix atained by removing the specified row and column and
		 * place the result into the provided matrix.
		 *
		 * @param row Row to remove
		 * @param col Column to remove
		 * @param result Matrix to write the result into
		 */
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
		/**
		 * Embed this matrix into the provided matrix, so that this matrix is the
		 * first submatrix. All other values are set to 0.
		 *
		 * @param result Matrix to wrtie the result into.
		 */
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
		/** Number of rows in the matrix. */
		int num_rows_;
		/** Number of columns in the matrix. */
		int num_cols_;
		/** The data in the matrix. */
		std::vector<int> data_;

	 private:
		/** Cached hashcode for the matrix. */
		std::size_t hashcode_;

		/** Get the index in the array for the provided position. */
		int get_index(const int row, const int col) const;
		/** Compute the hash for this matrix. */
		virtual std::size_t compute_hash() const;

		/** Multiply left and right together and put the result into result. */
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


