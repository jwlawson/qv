/**
 * quiver_matrix.h
 * Copyright 2014-2015 John Lawson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * QuiverMatrix is a basic integer matrix wth added methods to mutate the
 * matrix at one of its vertices. There are also convenience methods for
 * finding the subquiver of the matrix, by removing one of its vertices which
 * ensures that the quiver returned does not contain any disconnected vertices.
 */
#pragma once
#include "int_matrix.h"
#include <cstdlib> /* std::abs */

namespace cluster {
	class QuiverMatrix : public IntMatrix {
	 public:
		 /** Construct a default QuiverMatrix of size 0. */
		QuiverMatrix();
		/**
		 * Construct a QuiverMatrix of specified size, but full of zeros.
		 * @param rows Number of rows in matrix
		 * @param cols Number of columns in matrix
		 */
		QuiverMatrix(const int rows, const int cols);
		/**
		 * Construct a QuiverMatrix of specified size and fill with the values
		 * provided in the array.
		 * 
		 * @param rows Number of rows in matrix
		 * @param cols Number of columns in matrix
		 * @param values Data to put into the matrix
		 */
		QuiverMatrix(const int rows, const int cols, const int values[]);
		/**
		 * Copy constructor. Copies all data from the specified matrix.
		 * @param mat Matrix to copy
		 */
		QuiverMatrix(const QuiverMatrix &mat) = default;
		/**
		 * Similar to the copy constructor, this method takes a matrix which the
		 * compiler has already copied and sets this matrix to that.
		 * @param matrix Matrix to copy
		 */
		QuiverMatrix(IntMatrix const& matrix);
		/**
		 * Move constructor.
		 * @param matrix Matrix to move into this one
		 */
		QuiverMatrix(QuiverMatrix&&) = default;
		/**
		 * Create a QuiverMatrix from a string. The string is expected to be
		 * formatted like one from the << method.
		 *
		 * @see IntMatrix#IntMatrix(std::String)
		 * @param str String containing matrix information
		 */
		QuiverMatrix(std::string const& str);
		virtual ~QuiverMatrix() {};
		/**
		 * Check whether this matrix is definitely mutation infinite.
		 * As no information is known about the mutation class, this can only check
		 * the data in theis matrix.
		 * @return true if the matrix is mutaion infinite
		 */
		bool is_infinite() const;
		/**
		 * Assignment operator.
		 * @param mat Matrix to set this to
		 */
		QuiverMatrix &operator=(QuiverMatrix const& mat) = default;
		QuiverMatrix &operator=(QuiverMatrix && mat) = default;

		/**
		 * Mutate this matrix at the specified vertex. The resulting matrix is
		 * stored in the provided matrix.
		 * @param k Vertex to mutate at
		 * @param result Matrix to store result in
		 */
		template<class T>
		void mutate(const int k, T& result) const;
		/**
		 * Find the subquiver by removing the vertex specified.
		 *
		 * If by removing the vertex there is an unconnected vertex in the
		 * subquiver, this vertex is removed. Be careful as this can mean that the
		 * quiver returned contains no vertices and is really just an empty matrix.
		 *
		 * @param k Vertex to remove
		 * @param result Matrix to store result in
		 */
		template<class T>
		void subquiver(const int k, T& result) const;
	 private:
		/** Cached col vector to write mutation data to. */
		static std::vector<int> k_abs_row_;
	};
	template<class T>
	inline
	void QuiverMatrix::mutate(const int k, T& result) const {
		int const * k_row = data_.data() + k * num_cols_;
		k_abs_row_.reserve(num_cols_);
		for(int i = 0; i < num_cols_; ++i) {
			k_abs_row_[i] = std::abs(k_row[i]);
		}
		int const * this_data = data_.data();
		int * result_data = result.data_.data();
		for(int i = 0; i < num_rows_; ++i) {
			for(int j = 0; j < num_cols_; ++j, ++this_data, ++result_data) {
				if(i == k || j == k) {
					*result_data= -(*this_data);
				} else {
				*result_data = *this_data +
					(k_abs_row_[i] * k_row[j] - k_row[i] * k_abs_row_[j]) / 2;
					/*(k_row[i] < 0 ? -1 : 1) * std::min(k_row[i] * k_row[j], 0);*/
				}
			}
		}
		result.reset();
	}
	template<class T>
	inline
	void QuiverMatrix::subquiver(const int k, T& result) const {
		submatrix(k, k, result);
/*		int zero = result.zero_row();
		if (zero != -1) {
			T tmp(result.num_rows() - 1, result.num_cols() - 1);
			result.subquiver(zero, tmp);
			result=std::move(tmp);
		}
		result.reset();
*/
	}
}

namespace std {
	/* Add hash function to the std::hash struct. */
	template <>
	struct hash<cluster::QuiverMatrix> {
		size_t operator()(const cluster::QuiverMatrix &x) const {
			return x.hash();
		}
	};
	template <>
	struct hash<std::shared_ptr<cluster::QuiverMatrix>> {
		size_t operator()(const std::shared_ptr<cluster::QuiverMatrix> &x) const {
			return x->hash();
		}
	};
	/* Add equals function to std::equal_to */
	template<>
	struct equal_to<cluster::QuiverMatrix> {
		bool operator()(const cluster::QuiverMatrix &lhs,
		                const cluster::QuiverMatrix &rhs) const {
			return lhs.equals(rhs);
		}
	};
	template<>
	struct equal_to<std::shared_ptr<cluster::QuiverMatrix>> {
		bool operator()(const std::shared_ptr<cluster::QuiverMatrix> &lhs,
		                const std::shared_ptr<cluster::QuiverMatrix> &rhs) const {
			return lhs->equals(*rhs);
		}
	};
}

