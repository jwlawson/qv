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
		EquivMutationClass(const M initial);
	 protected:
		virtual void seen_matrix(const M &mat, const M &previous, const int vertex);
		virtual void unseen_matrix(const M &matrix, const M &previous,
		                           const int vertex);
		virtual bool have_seen(const M &matrix);

	 private:
		std::unordered_set<EquivQuiverMatrix> set_;
	};
}

