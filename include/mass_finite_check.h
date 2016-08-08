/*
 * mass_finite_check.h
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
 * Checks whether given matrices are mutation-finite. Initially the fast check is
 * used, and if necessary the whole mutation-class is computed. All matrices in
 * the class are then kept in a cache of mutation-finite quivers - so that the
 * class does not need to be repreatedly computed.
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
			MPtr last_checked_;

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

