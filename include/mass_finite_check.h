/*
 * mass_finite_check.h
 */
#pragma once

#include <memory>
#include <unordered_set>

#include "equiv_quiver_matrix.h"
#include "mutation_class.h"

namespace cluster {
	class MassFiniteCheck {
		private:
			typedef EquivQuiverMatrix M;
			typedef typename std::shared_ptr<M> MPtr;
			typedef typename std::unordered_set<MPtr> MSet;
		public:
			/**
			 * Create a new check object with empty set of computed matrices.
			 */
			MassFiniteCheck();
			/**
			 * Check whether the specified matrix is mutation-finite.
			 * @param matrix Matrix to check
			 * @return true if finite
			 */
			bool is_finite(const M& matrix);
			/**
			 * Checks whether the last matrix checked was from a previously calculated
			 * mutation class or not.
			 * @return true if the last matrix was from a new class
			 */
			bool last_new_class() const;
			/**
			 * Get the set containing all the finite matrices found.
			 */
			std::shared_ptr<const MSet> set() const;
			/**
			 * Add a matrix which is known to be finite to the cache.
			 * @param matrix Finite matrix to add
			 */
			void add_finite(const MPtr& matrix);

		private:
			/** Set of finite matrices. */
			std::shared_ptr<MSet> set_;
			/** true if the last checked matrix was in a new mutation class. */
			bool last_new_;

			/**
			 * Check whether the matrix has been computed to be finite previously.
			 * @param mat Matrix to check
			 * @return true if the matrix is known to be finite
			 */
			bool set_contains(MPtr mat) const;
			/**
			 * Add all matrices in the mutation class to the set.
			 * @param Finite mutation class to add
			 */
			void add_class(const MutationClass& c);
	};
}

