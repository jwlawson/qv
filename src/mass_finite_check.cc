/*
 * mass_finite_check.cc
 */
#include "mass_finite_check.h"

#include "fast_infinite.h"

namespace cluster {

	MassFiniteCheck::MassFiniteCheck()
		: set_() {}

	bool MassFiniteCheck::is_finite(const M& matrix) {
		MPtr p = std::make_shared<M>(matrix);
		if(set_contains(p)) {
			return true;
		}
		if(fastinf::is_infinite(matrix)) {
			return false;
		}
		MutationClass c(matrix);
		if(c.is_finite()) {
			add_class(c);
			return true;
		} else {
			return false;
		}
	}

	bool MassFiniteCheck::set_contains(MPtr mat) const {
		return set_.find(mat) != set_.end();
	}

	void MassFiniteCheck::add_class(const MutationClass& c){
		for(auto iter = c.begin(); iter != c.end(); iter++){
			set_.insert(*iter);
		}
	}

}

