/*
 * equiv_mutation_class_size.cc
 */

#include "equiv_mutation_class_size.h"

#include <iostream>

namespace cluster {
	namespace equivsize {

		int Size(const EquivQuiverMatrix& matrix) {
			EquivMutationClass c(matrix);
			int result = 0;
			while (c.has_next()) {
				if(IntMatrix::are_equal(c.next(),
							MutationClass<EquivQuiverMatrix>::INFINITE)) {
					return -1;
				} else {
					result++;
				}
			}
			return result;
		}
	}
}


