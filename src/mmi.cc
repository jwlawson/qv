/*
 * mmi.cc
 */

#include "mmi.h"
#include "fast_infinite.h"
#include "submatrix_iterator.h"
#include "equiv_quiver_matrix.h"
#include "equiv_mutation_class_size.h"
#include <iostream>

namespace cluster {
	namespace mmi {

		bool fast_mmi(QuiverMatrix matrix) {

			if(!fastinf::is_infinite(matrix)) {
				return false;
			} 
			SubmatrixIterator<QuiverMatrix> iter(matrix);
			while(iter.has_next()) {
				QuiverMatrix n = std::move(iter.next());
				if(fastinf::is_infinite(n)){
					return false;
				}
			}
			return true;
		}

		bool slow_mmi(QuiverMatrix matrix) {

			if(!fastinf::is_infinite(matrix)) {
				/* Matrix probably finite. */
				EquivQuiverMatrix e(matrix);
				if(equivsize::Size(e) > 0) {
					/* Actually finite. */
					return false;
				}
			}
			/* Now check submatrices. */
			SubmatrixIterator<QuiverMatrix> iter(matrix);
			while(iter.has_next()) {
				QuiverMatrix n = std::move(iter.next());
				if(fastinf::is_infinite(n)){
					return false;
				}
				/* Probably finite. */
				EquivQuiverMatrix e(n);
				if(equivsize::Size(e) < 0) {
					/* Actually infinite. */
					return false;
				}
			}
			return true;
		}
	}
}

