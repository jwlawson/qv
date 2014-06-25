/*
 * equiv_mutation_class_loader.h
 */
#pragma once

#include "mutation_class_loader.h"

#include <unordered_set>

#include "equiv_quiver_matrix.h"

namespace cluster {
	class EquivMutationClassLoader : public MutationClassLoader<EquivQuiverMatrix> {
	private:
		typedef EquivQuiverMatrix M;
	public:
		EquivMutationClassLoader();
		EquivMutationClassLoader(const M& initial);
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

