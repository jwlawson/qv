/*
 * equiv_quiver_matrix.h
 *
 * Contains the EquivQuiverMatrix class which is a QuiverMatrix whose equals
 * method has been changed so that matrices which are the same up to permuting
 * their rows and columns are considered equal.
 */
#pragma once

#include "quiver_matrix.h"
#include "equivalence_checker.h"

namespace cluster {
	class EquivQuiverMatrix : public QuiverMatrix {
		public:
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
			EquivQuiverMatrix(const EquivQuiverMatrix& mat);
			/**
			 * Move constructor.
			 * @param mat Matrix to move into this one.
			 */
			EquivQuiverMatrix(EquivQuiverMatrix&& mat);
			/**
			 * Create a EquivQuiverMatrix from a string. The string is expected to be
			 * formatted like one from the << method.
			 *
			 * @see IntMatrix#IntMatrix(std::String)
			 * @param str String containing matrix information
			 */
			EquivQuiverMatrix(std::string str);
			/** Overwritten equals method which ensures that two matrices which are
			 * the same up to permuting their rows and columns are considered equal.
			 *
			 * @param mat Matrix to check
			 * @return true if equal up to permuting rows and columns
			 */
			virtual bool equals(const IntMatrix &mat) const;
			virtual void reset();
			/**
			 * Assignment operator.
			 */
			EquivQuiverMatrix &operator=(EquivQuiverMatrix mat);

		protected:
			/** Overwritten hash function. */
			virtual std::size_t compute_hash() const;

		private:
			std::vector<std::pair<int, int>> rows_;
			std::vector<std::pair<int, int>> cols_;
			/** Equivalence checker to check if matrices are equivalent. */
			std::shared_ptr<EquivalenceChecker> checker_;

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

