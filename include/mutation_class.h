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
			MutationClass(const IntMatrix& initial);
			MutationClass(const EquivQuiverMatrix& initial);

			bool contains(std::shared_ptr<EquivQuiverMatrix> matrix) const;
			bool is_finite() const;
			std::size_t size() const;
			Iter begin() const;
			Iter end() const;

		private:
			std::unordered_set<MPtr> set_;
			bool finite_;
	};
}
