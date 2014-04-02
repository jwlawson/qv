#include <cstdlib>
#include "equiv_quiver_matrix.h"
#include "array_utils.h"
#include <algorithm>

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
	EquivQuiverMatrix::EquivQuiverMatrix(IntMatrix matrix)
		: QuiverMatrix(matrix) {
		checker_ = EquivalenceChecker::Get(matrix.num_rows());
	}
	EquivQuiverMatrix::~EquivQuiverMatrix() {}
	bool EquivQuiverMatrix::equals(const IntMatrix &mat) const {
		return checker_->are_equivalent(*this, mat);
	}


	EquivQuiverMatrix &EquivQuiverMatrix::operator=(EquivQuiverMatrix mat) {
		IntMatrix::operator=(mat);
		return *this;
	}

	/* Private methods */

	std::size_t EquivQuiverMatrix::compute_hash() const {
		std::size_t hash = 137;
		std::vector<int> rowSum(num_rows(), 0);
		std::vector<int> colSum(num_cols(), 0);
		std::vector<int> absRowSum(num_rows(), 0);
		std::vector<int> absColSum(num_cols(), 0);
		for (int i = 0; i < num_rows(); i++) {
			for (int j = 0; j < num_cols(); j++) {
				rowSum[i] += get(i, j);
				colSum[j] += get(i, j);
				absRowSum[i] += std::abs(get(i, j));
				absColSum[j] += std::abs(get(i, j));
			}
		}
		std::sort(rowSum.begin(), rowSum.end());
		std::sort(absRowSum.begin(), absRowSum.end());
		std::sort(colSum.begin(), colSum.end());
		std::sort(absColSum.begin(), absColSum.end());
		hash += 257 * arrays::hash(rowSum);
		hash += 73 * arrays::hash(colSum);
		hash += 67 * arrays::hash(absRowSum);
		hash += 157 * arrays::hash(absColSum);

		return hash;
	}
}
