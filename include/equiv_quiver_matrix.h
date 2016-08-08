/*
 * equiv_quiver_matrix.h
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
 * Contains the EquivQuiverMatrix class which is a QuiverMatrix whose equals
 * method has been changed so that matrices which are the same up to permuting
 * their rows and columns are considered equal.
 */
#pragma once

#include "quiver_matrix.h"
#include "equivalence_checker.h"

namespace cluster {
	/* Need to forward reference to prevent circular references. */
	class EquivalenceChecker;

	class EquivQuiverMatrix : public QuiverMatrix {
		public:
			typedef std::vector<std::pair<int, int>> PairVector;
			typedef std::vector<int> Permutation;
			typedef const Permutation & CPermutation;
			typedef std::vector<Permutation> PermVec;
			typedef std::shared_ptr<PermVec> PermVecPtr;
			/** 
			 * Create a default matrix of size 0.
			 */
			EquivQuiverMatrix();
			/**
			 * Create a matrix of specified size and with all values set to 0.
			 *
			 * @param rows Number of rows
			 * @param cols Number of columns
			 */
			EquivQuiverMatrix(const int rows, const int cols);
			/**
			 * Create a matrix with specified values.
			 *
			 * The values in the array should be in row major form, that is all the
			 * entries of the first row, then the second row etc.
			 *
			 * @param rows Number of rows
			 * @param cols Numer of columns
			 * @param values Values to put into the matrix
			 */
			EquivQuiverMatrix(const int rows, const int cols, const int values[]);
			/**
			 * Copy constructor for the subclass QuiverMatrix.
			 *
			 * Useful for making EquivQuiverMatrices from the matrices provided in
			 * Dynkin.
			 *
			 * @param mat Matrix to copy
			 */
			EquivQuiverMatrix(const IntMatrix& mat);
			/**
			 * Copy constructor.
			 * @param mat Matrix to copy
			 */
			EquivQuiverMatrix(const EquivQuiverMatrix& mat) = default;
			/**
			 * Move constructor.
			 * @param mat Matrix to move into this one.
			 */
			EquivQuiverMatrix(EquivQuiverMatrix&& mat) = default;
			/**
			 * Create a EquivQuiverMatrix from a string. The string is expected to be
			 * formatted like one from the << method.
			 *
			 * @see IntMatrix#IntMatrix(std::String)
			 * @param str String containing matrix information
			 */
			EquivQuiverMatrix(std::string const& str);
			virtual ~EquivQuiverMatrix(){};
			/**
			 * Overwrite the set method in IntMatrix to allow the PairVector objects
			 * to be initialised to the correct size.
			 * @param mat Matrix to set this one to
			 */
			virtual void set_matrix(const IntMatrix& mat);
			/** Overwritten equals method which ensures that two matrices which are
			 * the same up to permuting their rows and columns are considered equal.
			 *
			 * @param mat Matrix to check
			 * @return true if equal up to permuting rows and columns
			 */
			virtual bool equals(const IntMatrix &mat) const;
			/** Want a specialised equals for EquivQuiverMatrix to ensure that the
			 * copy in static_cast used in the IntMatrix equals method is not used.
			 */
			bool equals(const EquivQuiverMatrix &mat) const;
			virtual void reset();
			/**
			 * Assignment operator.
			 */
			EquivQuiverMatrix& operator=(const EquivQuiverMatrix& mat) = default;
			EquivQuiverMatrix& operator=(EquivQuiverMatrix && mat) = default;
			/**
			 * Get the permutation taking this matrix to the given equivalent matrix.
			 */
			CPermutation get_permutation(const IntMatrix& mat) const;
			/**
			 * Get a vector of all permutations taking this matrix to the given
			 * equivalent matrix.
			 * This assumes that the matrix supplied is in fact equivalent to this
			 * one, if not then problems could occur.
			 */
			PermVecPtr all_permutations(const EquivQuiverMatrix& mat) const;

			/** Vector holding pairs of row sums and the abs row sums. */
			PairVector rows_;
		protected:
			/** Overwritten hash function. */
			virtual std::size_t compute_hash() const;

		private:
			/** Equivalence checker to check if matrices are equivalent. */
			std::shared_ptr<EquivalenceChecker> checker_;
			/** Cached sorted row sums and abs row sums. */
			mutable PairVector sorted_rows_;

	};
}

namespace std {
	/* Add hash function to the std::hash struct. */
	template <>
	struct hash<cluster::EquivQuiverMatrix> {
		size_t operator()(const cluster::EquivQuiverMatrix &x) const {
			return x.hash();
		}
	};
	/**
	 * Annoyingly the standard shared_ptr hash function which passes the
	 * function to the object in the pointer doesn't seem to work.
	 */
	template <>
	struct hash<std::shared_ptr<cluster::EquivQuiverMatrix>> {
		size_t operator()(const std::shared_ptr<cluster::EquivQuiverMatrix> &x)
				const {
			return x->hash();
		}
	};
	/* Add equals function to std::equal_to */
	template<>
	struct equal_to<cluster::EquivQuiverMatrix> {
		bool operator()(const cluster::EquivQuiverMatrix &lhs,
		                const cluster::EquivQuiverMatrix &rhs) const {
			return lhs.equals(rhs);
		}
	};
	template<>
	struct equal_to<std::shared_ptr<cluster::EquivQuiverMatrix>> {
		bool operator()(const std::shared_ptr<cluster::EquivQuiverMatrix> &lhs,
		                const std::shared_ptr<cluster::EquivQuiverMatrix> &rhs) const {
			return lhs->equals(*rhs);
		}
	};
}

