/*
 * class_ext_iterator.h
 */

#include "equiv_mutation_class.h"
#include "extension_iterator.h"

namespace cluster {
	class EquivMutClassExtIterator {

		public:
			EquivMutClassExtIterator(const QuiverMatrix& matrix);

			QuiverMatrix next();
			bool has_next();

		private:
			QuiverMatrix matrix_;
			EquivMutationClass class_;
			ExtensionIterator<QuiverMatrix> iter_;

	};
}

