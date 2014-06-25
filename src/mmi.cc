/*
 * mmi.cc
 */
#include "mmi.h"

#include "equiv_mutation_class_size.h"
#include "equiv_quiver_matrix.h"
#include "fast_infinite.h"
#include "mass_finite_check.h"
#include "submatrix_iterator.h"

namespace cluster {
	namespace mmi {

		MassFiniteCheck chk;

		void add_finite(const std::shared_ptr<EquivQuiverMatrix>& mat) {
			chk.add_finite(mat);
		}

		bool fast_mmi(const QuiverMatrix& matrix) {

			if(!fastinf::is_infinite(matrix)) {
				EquivQuiverMatrix e(matrix);
				if(chk.is_finite(e)) {
					/* Actually finite. */
					return false;
				}
			} 
			SubmatrixIterator<QuiverMatrix> iter(matrix);
			typedef std::shared_ptr<QuiverMatrix> MatPtr;
			while(iter.has_next()) {
				MatPtr n = std::move(iter.next());
				if(fastinf::is_infinite(*n)){
					return false;
				}
			}
			return true;
		}

		bool slow_mmi(const QuiverMatrix& matrix) {

			if(!fastinf::is_infinite(matrix)) {
				/* Matrix probably finite. */
				EquivQuiverMatrix e(matrix);
				if(chk.is_finite(e)) {
					/* Actually finite. */
					return false;
				}
			}
			/* Now check submatrices. */
			typedef std::shared_ptr<QuiverMatrix> MatPtr;
			SubmatrixIterator<QuiverMatrix> iter(matrix);
			while(iter.has_next()) {
				MatPtr n = std::move(iter.next());
				if(fastinf::is_infinite(*n)){
					return false;
				}
				/* Probably finite. */
				EquivQuiverMatrix e(*n);
				if(!chk.is_finite(e)) {
					/* Actually infinite. */
					return false;
				}
			}
			return true;
		}
	}
}

