/*
 * equiv_mutation_class.h
 */
#pragma once

#include "mutation_class.h"

#include <unordered_set>

#include "equiv_quiver_matrix.h"

namespace cluster {
	class EquivMutationClass : public MutationClass<EquivQuiverMatrix> {
	private:
		typedef EquivQuiverMatrix M;
	public:
		EquivMutationClass(const M& initial);
	protected:
		virtual void seen_matrix(std::shared_ptr<M> mat,
														 std::shared_ptr<M> previous,
														 const int vertex);
		virtual void unseen_matrix(std::shared_ptr<M> matrix,
															 std::shared_ptr<M> previous,
		                           const int vertex);
		virtual bool have_seen(std::shared_ptr<M> matrix);

	private:
		std::unordered_set<std::shared_ptr<M>> set_;
	};
}

