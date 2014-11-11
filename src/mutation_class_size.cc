/*
 * mutation_class_size.cc
 */
#include "mutation_class_size.h"
#include "mutation_class_loader.h"

namespace cluster {
	namespace classsize {
		int Size(const QuiverMatrix& matrix) {
			MutationClassLoader<QuiverMatrix> c(matrix);
			int result = 0;
			while (c.has_next()) {
				if (c.next_ptr()->equals(MutationClassLoader<QuiverMatrix>::INFINITE)) {
					return -1;
				}
				result++;
			}
			return result;
		}
	}
}

