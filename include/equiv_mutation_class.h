/*
 * equiv_mutation_class.h
 */

#include "mutation_class.h"
#include "equiv_quiver_matrix.h"
#include <unordered_set>

namespace cluster {
	class EquivMutationClass : public MutationClass<EquivQuiverMatrix> {
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

