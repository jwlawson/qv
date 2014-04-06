/*
 * equiv_mutation_class.cc
 */

#include "equiv_mutation_class.h"
#include "int_matrix.h"

namespace cluster {

	EquivMutationClass::EquivMutationClass(const EquivQuiverMatrix& matrix)
		: MutationClass<EquivQuiverMatrix>(matrix),
		  set_() {
		set_.insert(matrix_);
	}

	void EquivMutationClass::seen_matrix(std::shared_ptr<M> mat,
	                                     std::shared_ptr<M> previous,
																			 const int vertex) {
		map_[previous].link(vertex);
		if (map_.count(mat) > 0 && IntMatrix::are_equal(*mat, *map_[mat].matrix())) {
			/* New matrix is the same not just equivalent to the one seen before. */
			map_[mat].link(vertex);
		}
	}

	void EquivMutationClass::unseen_matrix(std::shared_ptr<M> mat,
	                                       std::shared_ptr<M> previous,
																				 const int vertex) {
		set_.insert(mat);
		MutationClass<EquivQuiverMatrix>::unseen_matrix(mat, previous, vertex);
	}

	bool EquivMutationClass::have_seen(std::shared_ptr<M> matrix) {
		return set_.count(matrix) > 0;
	}

}

