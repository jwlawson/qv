/*
 * equiv_quiver_matrix.cc
 */
#include "equiv_quiver_matrix.h"

#include <algorithm>

#include "array_utils.h"

namespace cluster {
	EquivQuiverMatrix::EquivQuiverMatrix() : QuiverMatrix() {
		checker_ = EquivalenceChecker::Get(0);
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const int rows, const int cols)
		: QuiverMatrix(rows, cols) {
		checker_ = EquivalenceChecker::Get(rows);
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const int rows, const int cols,
	                                     const int values[])
		: QuiverMatrix(rows, cols, values) {
		checker_ = EquivalenceChecker::Get(rows);
		reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const QuiverMatrix& matrix)
		: QuiverMatrix(static_cast<IntMatrix>(matrix)) {
		checker_ = EquivalenceChecker::Get(matrix.num_rows());
		reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const EquivQuiverMatrix& mat)
		: QuiverMatrix(static_cast<IntMatrix>(mat)) {
			checker_ = mat.checker_;
			reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(EquivQuiverMatrix&& mat) 
		: QuiverMatrix(std::move(mat)) {
			checker_ = EquivalenceChecker::Get(num_rows_);
	}

	EquivQuiverMatrix::EquivQuiverMatrix(std::string str) 
		: QuiverMatrix(str) {
			checker_ = EquivalenceChecker::Get(num_rows_);
			reset();
	}

	bool EquivQuiverMatrix::equals(const IntMatrix &mat) const {
		return checker_->are_equivalent(*this, mat);
	}

	EquivQuiverMatrix &EquivQuiverMatrix::operator=(EquivQuiverMatrix mat) {
		IntMatrix::operator=(mat);
		checker_ = mat.checker_;
		return *this;
	}

	/* Private methods */

	std::size_t EquivQuiverMatrix::compute_hash() const {
		std::size_t hash = 137;
		std::vector<std::pair<int, int>> rows(num_rows_, std::make_pair(0, 0));
		std::vector<std::pair<int, int>> cols(num_cols_, std::make_pair(0, 0));

		for (int i = 0; i < num_rows(); i++) {
			for (int j = 0; j < num_cols(); j++) {
				rows[i].first += get(i,j);
				rows[i].second += std::abs(get(i,j));
				cols[j].first += get(i,j);
				cols[j].second += std::abs(get(i,j));
			}
		}
		std::sort(rows.begin(), rows.end());
		std::sort(cols.begin(), cols.end());
		hash += 257 * arrays::hash(rows);
		hash += 73 * arrays::hash(cols);

		return hash;
	}
}
