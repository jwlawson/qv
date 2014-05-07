/*
 * class_ext_iterator.cc
 */
#include "class_ext_iterator.h"

namespace cluster {
	EquivMutClassExtIterator::EquivMutClassExtIterator(const QuiverMatrix& mat)
		: class_(mat),
			iter_() {}

	QuiverMatrix EquivMutClassExtIterator::next() {
		if(!iter_.has_next()) {
			iter_ = ExtensionIterator<QuiverMatrix>(class_.next());
		}
		return std::move(iter_.next());
	}

	bool EquivMutClassExtIterator::has_next() {
		return iter_.has_next() || class_.has_next();
	}

}

