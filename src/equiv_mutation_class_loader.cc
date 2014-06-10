/*
 * equiv_mutation_class_loader.cc
 */

#include "equiv_mutation_class_loader.h"
#include "int_matrix.h"

namespace cluster {

	EquivMutationClassLoader::EquivMutationClassLoader(const EquivQuiverMatrix& matrix)
		: MutationClassLoader<EquivQuiverMatrix>(matrix),
		  set_() {
		set_.insert(matrix_);
	}

	void EquivMutationClassLoader::seen_matrix(std::shared_ptr<M> mat,
	                                     std::shared_ptr<M> previous,
																			 const int vertex) {
		map_[previous].link(vertex);
		if (map_.count(mat) > 0) {
			/*
			 * Cannot rely just on count to see whether the matrix is in the map.
			 * This seems to be bugged and one in many thousands of matrices will
			 * appear to be in the map before the count call, but afterwards are not.
			 * Of the few of these cases I have seen the matrix should not have had
			 * count return 1, but for some reason it did.
			 *
			 * Checking the result of find after the call to count *seems* to fix this
			 * bug.
			 *
			 * See also equiv_mutation_class_size_test.cc for an example of such a
			 * matrix.
			 */
			auto ref = map_.find(mat);
			if(ref != map_.end() && IntMatrix::are_equal(*mat, *(ref->first))) {
				/* New matrix is the same not just equivalent to the one seen before. */
				map_[mat].link(vertex);
			}
		}
	}

	void EquivMutationClassLoader::unseen_matrix(std::shared_ptr<M> mat,
	                                       std::shared_ptr<M> previous,
																				 const int vertex) {
		set_.insert(mat);
		MutationClassLoader<EquivQuiverMatrix>::unseen_matrix(mat, previous, vertex);
	}

	bool EquivMutationClassLoader::have_seen(std::shared_ptr<M> matrix) {
		return set_.count(matrix) > 0;
	}

}

