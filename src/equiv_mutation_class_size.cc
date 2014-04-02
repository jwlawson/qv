/*
 * equiv_mutation_class_size.cc
 */

#include "equiv_mutation_class_size.h"

namespace cluster {
	namespace equivsize {

		int Size(const EquivQuiverMatrix matrix) {
			EquivMutationClass c(matrix);
			int result = 0;
			while (c.has_next()) {
				c.next();
				result++;
			}
			return result;
		}
	}
}


