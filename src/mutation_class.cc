/*
 * mutation_class.cc
 */
#include "mutation_class.h"

#include "equiv_mutation_class_loader.h"
#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {

	MutationClass::MutationClass(const IntMatrix& initial)
		: set_(), 
		 	finite_(true) {
		EquivQuiverMatrix mat(initial);
		EquivMutationClassLoader loader(mat);
		while(loader.has_next()){
			MPtr p = loader.next_ptr();
			if((*p).equals(EquivMutationClassLoader::INFINITE)) {
				finite_ = false;
				break;
			}
			set_.insert(p);
		}
	}
	/*
	 * Don't need to create a new EquivQuiverMatrix if that is what is supplied.
	 */
	MutationClass::MutationClass(const EquivQuiverMatrix& initial) 
		: set_(), 
			finite_(true) {
		EquivMutationClassLoader loader(initial);
		while(loader.has_next()){
			MPtr p = loader.next_ptr();
			if((*p).equals(EquivMutationClassLoader::INFINITE)) {
				finite_ = false;
				break;
			}
			set_.insert(p);
		}
	}

	bool MutationClass::contains(std::shared_ptr<M> matrix) const {
		return set_.find(matrix) != set_.end();
	}

	bool MutationClass::is_finite() const {
		return finite_;
	}

	std::size_t MutationClass::size() const {
		return set_.size();
	}

	typename MutationClass::Iter MutationClass::begin() const {
		return set_.begin();
	}

	typename MutationClass::Iter MutationClass::end() const {
		return set_.end();
	}

}


