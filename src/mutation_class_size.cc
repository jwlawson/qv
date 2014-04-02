/*
 * mutation_class_size.cc
 */

#include "mutation_class_size.h"
#include "mutation_class.h"
#include <iostream>

namespace cluster {
	namespace classsize {
		int Size(QuiverMatrix matrix) {
			MutationClass<QuiverMatrix> c(matrix);
			int result = 0;
			while (c.has_next()) {
				if (c.next().equals(MutationClass<QuiverMatrix>::INFINITE)) {
					return -1;
				}
				result++;
			}
			return result;
		}
	}
}

