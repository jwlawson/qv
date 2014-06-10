/*
 * mutation_class.h
 */
#pragma once

#include <memory>
#include <unordered_set>

#include "equiv_quiver_matrix.h"
#include "mutation_class_loader.h"

namespace cluster {
	class MutationClass {
		private:
			typedef EquivQuiverMatrix M;
			typedef typename std::shared_ptr<M> MPtr;
			typedef typename std::unordered_set<MPtr> MSet;
			typedef typename MSet::const_iterator Iter;

		public:
			/**
			 * Create a new MutationClass from this inital matrix.
			 *
			 * The whole mutation class is computed, which for larger matrices can
			 * take a significant amount of time.
			 * @param initial The initial matrix in the mutation class.
			 */
			MutationClass(const IntMatrix& initial);
			/**
			 * Same as MutationClass(const IntMatrix&), but does not need to create a
			 * new EquivQuiverMatrix object.
			 */
			MutationClass(const EquivQuiverMatrix& initial);

			/**
			 * Check whether the mutation class contains a specific matrix.
			 * @param matrix Matrix to check.
			 * @return true if the matrix is in the class.
			 */
			bool contains(std::shared_ptr<EquivQuiverMatrix> matrix) const;
			/**
			 * Check if the mutation class is finite.
			 * @return true if the class is finite.
			 */
			bool is_finite() const;
			/**
			 * Get the number of matrices in the mutation class.
			 * @return The size of the class.
			 */
			std::size_t size() const;
			/**
			 * Get an (unordered) forward iterator over all matrices in the class.
			 * @return Forward iterator.
			 */
			Iter begin() const;
			/**
			 * Get an iterator pointing to the end of the mutation class.
			 * @return Iterator at end.
			 */
			Iter end() const;

		private:
			/** 
			 * The set containing all matrices in the mutation class.
			 *
			 * If the class is infinite then this contains the matrices which were
			 * encountered before a definitely infinite matrix was found.
			 */
			MSet set_;
			/** Whether the class is finite or not. */
			bool finite_;
	};
}
