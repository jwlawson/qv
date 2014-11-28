/*
 * equiv_quiver_matrix.cc
 */
#include "equiv_quiver_matrix.h"

#include <algorithm>

#include "array_utils.h"

namespace cluster {
	namespace {
		typedef std::vector<std::pair<int, int>> PairVector;
		typedef std::vector<int> Permutation;
		typedef std::vector<Permutation> PermVec;
		typedef std::shared_ptr<PermVec> PermVecPtr;
	}
	EquivQuiverMatrix::EquivQuiverMatrix()
		: QuiverMatrix(),
			rows_(),
			cols_() {
		checker_ = EquivalenceChecker::Get(0);
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const int rows, const int cols)
		: QuiverMatrix(rows, cols),
			rows_(rows, std::make_pair(0,0)),
			cols_(cols, std::make_pair(0,0)) {
		checker_ = EquivalenceChecker::Get(rows);
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const int rows, const int cols,
	                                     const int values[])
		: QuiverMatrix(rows, cols, values),
			rows_(rows, std::make_pair(0,0)),
			cols_(cols, std::make_pair(0,0)) {
		checker_ = EquivalenceChecker::Get(rows);
		reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const IntMatrix& matrix)
		: QuiverMatrix(matrix),
			rows_(matrix.num_rows(), std::make_pair(0,0)),
			cols_(matrix.num_cols(), std::make_pair(0,0)) {
		checker_ = EquivalenceChecker::Get(matrix.num_rows());
		reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(const EquivQuiverMatrix& mat)
		: QuiverMatrix(reinterpret_cast<const IntMatrix&>(mat)),
			rows_(mat.num_rows(), std::make_pair(0,0)),
			cols_(mat.num_cols(), std::make_pair(0,0)) {
		checker_ = EquivalenceChecker::Get(mat.num_rows());
		reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(EquivQuiverMatrix&& mat) 
		: QuiverMatrix(std::move(mat)),
			rows_(mat.num_rows(), std::make_pair(0,0)),
			cols_(mat.num_cols(), std::make_pair(0,0)) {
			checker_ = EquivalenceChecker::Get(num_rows_);
			reset();
	}

	EquivQuiverMatrix::EquivQuiverMatrix(std::string str) 
		: QuiverMatrix(str),
			rows_(num_rows(), std::make_pair(0,0)),
			cols_(num_cols(), std::make_pair(0,0))  {
			checker_ = EquivalenceChecker::Get(num_rows_);
			reset();
	}

	void EquivQuiverMatrix::set_matrix(const IntMatrix& mat) {
		if(mat.num_rows() != num_rows_) {
			rows_ = PairVector(mat.num_rows(), std::make_pair(0,0));
		}
		if(mat.num_cols() != num_cols_) {
			cols_ = PairVector(mat.num_cols(), std::make_pair(0,0));
		}
		IntMatrix::set_matrix(mat);
	}

	/* Note that the static_cast will create a copy of mat */
	bool EquivQuiverMatrix::equals(const IntMatrix &mat) const {
		return checker_->are_equivalent(*this, 
				static_cast<EquivQuiverMatrix>(mat));
	}
	bool EquivQuiverMatrix::equals(const EquivQuiverMatrix &mat) const {
		return checker_->are_equivalent(*this, mat);
	}

	void EquivQuiverMatrix::reset() {
		for(size_t i = 0; i < rows_.size(); ++i) {
			rows_[i].first = 0;
			rows_[i].second = 0;
		}
		for(size_t i = 0; i < cols_.size(); ++i) {
			cols_[i].first = 0;
			cols_[i].second = 0;
		}
		int row_i = 0;
		int col_i = 0;
		int i = 0;
		while(i < num_rows_ * num_cols_) {
			rows_[row_i].first += data_[i];
			rows_[row_i].second += std::abs(data_[i]);
			cols_[col_i].first += data_[i];
			cols_[col_i].second += std::abs(data_[i]);

			i++;
			col_i++;
			if(col_i == num_cols_) {
				col_i = 0;
				row_i++;
			}
		}
		IntMatrix::reset();
	}
	Permutation EquivQuiverMatrix::get_permutation(const IntMatrix& mat) const {
		this->equals(mat);
		return std::move(checker_->last_row_map());
	}
	PermVecPtr EquivQuiverMatrix::all_permutations(const EquivQuiverMatrix& mat) {
		return checker_->valid_row_maps(*this, mat);
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
