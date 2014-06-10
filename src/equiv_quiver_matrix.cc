/*
 * equiv_quiver_matrix.cc
 */
#include "equiv_quiver_matrix.h"

#include <algorithm>

#include "array_utils.h"

namespace cluster {
	EquivQuiverMatrix::EquivQuiverMatrix()
		: QuiverMatrix(),
			rows_(),
			cols_() {
		checker_ = EquivalenceChecker::Get(0);
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const int rows, const int cols)
		: QuiverMatrix(rows, cols),
			rows_(rows),
			cols_(cols) {
		checker_ = EquivalenceChecker::Get(rows);
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const int rows, const int cols,
	                                     const int values[])
		: QuiverMatrix(rows, cols, values),
			rows_(rows),
			cols_(cols) {
		checker_ = EquivalenceChecker::Get(rows);
		reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const IntMatrix& matrix)
		: QuiverMatrix(matrix),
			rows_(matrix.num_rows()),
			cols_(matrix.num_cols()) {
		checker_ = EquivalenceChecker::Get(matrix.num_rows());
		reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const EquivQuiverMatrix& mat)
		: QuiverMatrix(static_cast<IntMatrix>(mat)),
			rows_(mat.num_rows()),
			cols_(mat.num_cols()) {
		checker_ = EquivalenceChecker::Get(mat.num_rows());
		reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(EquivQuiverMatrix&& mat) 
		: QuiverMatrix(std::move(mat)),
			rows_(mat.num_rows()),
			cols_(mat.num_cols()) {
			checker_ = EquivalenceChecker::Get(num_rows_);
			reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(std::string str) 
		: QuiverMatrix(str),
			rows_(num_rows()),
			cols_(num_cols())  {
			checker_ = EquivalenceChecker::Get(num_rows_);
			reset();
	}

	bool EquivQuiverMatrix::equals(const IntMatrix &mat) const {
		return checker_->are_equivalent(*this, mat);
	}

	void EquivQuiverMatrix::reset() {
		for (int i = 0; i < num_rows(); i++) {
			for (int j = 0; j < num_cols(); j++) {
				rows_[i].first += get(i,j);
				rows_[i].second += std::abs(get(i,j));
				cols_[j].first += get(i,j);
				cols_[j].second += std::abs(get(i,j));
			}
		}
		IntMatrix::reset();
	}

	EquivQuiverMatrix &EquivQuiverMatrix::operator=(EquivQuiverMatrix mat) {
		IntMatrix::operator=(mat);
		checker_ = mat.checker_;
		return *this;
	}

	/* Private methods */

	std::size_t EquivQuiverMatrix::compute_hash() const {
		std::size_t hash = 137;
		std::vector<std::pair<int, int>> rows(rows_);
		std::vector<std::pair<int, int>> cols(cols_);
		std::sort(rows.begin(), rows.end());
		std::sort(cols.begin(), cols.end());
		hash += 257 * arrays::hash(rows);
		hash += 73 * arrays::hash(cols);

		return hash;
	}
}
