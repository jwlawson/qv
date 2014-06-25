/*
 * class_ext_iterator.h
 *
 * Contains EquivMutClassExtIterator - a class to iterate over the whole
 * mutation class of the seed matrix and return every possible skew-symetric
 * extension of each member of the class.
 */
#pragma once

#include <memory>

#include "equiv_mutation_class_loader.h"
#include "extension_iterator.h"

namespace cluster {
	class EquivMutClassExtIterator {

		private:
			typedef EquivQuiverMatrix Matrix;
			typedef std::shared_ptr<Matrix> MatrixPtr;

		public:
			/**
			 * Default constructor. Does nothing much.
			 */
			EquivMutClassExtIterator();
			/**
			 * Create an iterator with the specified seed matrix. All extensions of
			 * all members in the matrix mutation class will be returned by the
			 * iterator.
			 * @param matrix Matrix to seed to the mutation class
			 */
			EquivMutClassExtIterator(const QuiverMatrix& matrix);

			/**
			 * Get the next matrix from the iterator.
			 */
			MatrixPtr next();
			/**
			 * Check whether the iterator has another matrix to return.
			 * @return true if next will return a valid matrix
			 */
			bool has_next();

		private:
			EquivMutationClassLoader class_;
			ExtensionIterator<QuiverMatrix> iter_;

	};
}

