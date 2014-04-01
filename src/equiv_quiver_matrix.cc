#include <cstdlib>
#include "equiv_quiver_matrix.h"
#include "array_utils.h"
#include <iostream>

namespace cluster {
	EquivQuiverMatrix::EquivQuiverMatrix()
		: QuiverMatrix() {}
	EquivQuiverMatrix::EquivQuiverMatrix(const int rows, const int cols)
		: QuiverMatrix(rows, cols) {
		checker_ = EquivalenceChecker::Get(rows);
	}
	EquivQuiverMatrix::EquivQuiverMatrix(const int rows, const int cols,
	                                     const int values[])
		: QuiverMatrix(rows, cols, values) {
		checker_ = EquivalenceChecker::Get(rows);
	}
	EquivQuiverMatrix::~EquivQuiverMatrix() {}
	bool EquivQuiverMatrix::equals(const IntMatrix &mat) const {
		return checker_->are_equivalent(*this, mat);
	}


		EquivQuiverMatrix& EquivQuiverMatrix::operator=(EquivQuiverMatrix mat){
			IntMatrix::operator=(mat);
			return *this;
		}

	/* Private methods */

	std::size_t EquivQuiverMatrix::compute_hash() const {
		std::size_t hash = 137;
		int *rowSum = new int[num_rows()];
		int *colSum = new int[num_cols()];
		int *absRowSum = new int[num_rows()];
		int *absColSum = new int[num_cols()];
		for (int i = 0; i < num_rows(); i++) {
			for (int j = 0; j < num_cols(); j++) {
				rowSum[i] += get(i, j);
				colSum[j] += get(i, j);
				absRowSum[i] += std::abs(get(i, j));
				absColSum[j] += std::abs(get(i, j));
			}
		}
		arrays::sort(rowSum, num_rows());
		arrays::sort(colSum, num_cols());
		arrays::sort(absRowSum, num_rows());
		arrays::sort(absColSum, num_cols());
		hash += 257 * arrays::hash(rowSum, num_rows());
		hash += 73 * arrays::hash(colSum, num_cols());
		hash += 67 * arrays::hash(absRowSum, num_rows());
		hash += 157 * arrays::hash(absColSum, num_cols());

		delete rowSum;
		delete colSum;
		delete absRowSum;
		delete absColSum;

		return hash;
	}
}
