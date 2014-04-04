#include "quiver_matrix.h"

namespace cluster {
	QuiverMatrix::QuiverMatrix()
		: IntMatrix() {}
	QuiverMatrix::QuiverMatrix(const int rows, const int cols)
		: IntMatrix(rows, cols) {}
	QuiverMatrix::QuiverMatrix(const int rows, const int cols, const int values[])
		: IntMatrix(rows, cols, values) {}
	QuiverMatrix::QuiverMatrix(const QuiverMatrix &mat)
		: IntMatrix(mat) {}
	QuiverMatrix::QuiverMatrix(IntMatrix matrix)
		: IntMatrix(matrix) {}
	QuiverMatrix::QuiverMatrix(QuiverMatrix &&mat)
		: IntMatrix(mat) {}

	QuiverMatrix &QuiverMatrix::operator=(QuiverMatrix mat) {
		IntMatrix::operator=(mat);
		return *this;
	}
	bool QuiverMatrix::is_infinite() const {
		for (int i = 0; i < num_rows_ * num_cols_; i++) {
			int val = data_[i];
			if (val >= 3 || val <= -3) {
				return true;
			}
		}
		return false;
	}

}
