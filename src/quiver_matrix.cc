#include "quiver_matrix.h"

namespace cluster {
	QuiverMatrix::QuiverMatrix(const int rows, const int cols) 
		: IntMatrix(rows,cols){}
	QuiverMatrix::QuiverMatrix(const int rows, const int cols, const int values[]) 
		: IntMatrix(rows, cols, values){}
	QuiverMatrix::~QuiverMatrix() {}
	const bool QuiverMatrix::is_infinite() const
	{
		for (int i = 0; i < num_rows(); i++) {
			for (int j = 0; j < num_cols(); j++) {
				int val = get(i, j);
				if (val >= 3 || val <= -3) {
					return true;
				}
			}
		}
		return false;
	}

}
