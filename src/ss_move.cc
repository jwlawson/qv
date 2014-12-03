/**
 * ss_move.cc
 */
#include "ss_move.h"

namespace cluster {
namespace {
typedef std::vector<SSMove::Applicable> AppVec;
}

AppVec SSMove::applicable_submatrices(const IntMatrix& m) const {
	return applicable_submatrices(std::make_shared<IntMatrix>(m));
}
/**
 * Check whether there is a row that only contains zeros, or values with the
 * same sign. That is every arrow at the vertex is pointing the same way.
 */
AppVec SSMove::applicable_submatrices(MatrixPtr m) const {
	AppVec result;
	int* r = m->data();
	int row = 0;
	int col = 0;
	int ind = 0;
	int max = m->num_rows() * m->num_cols();
	int maxcol = m->num_cols();
	int lastcol = maxcol - 1;
	bool pos = false;
	bool neg = false;
	while(ind < max) {
		if(col == 0) {
			pos = false;
			neg = false;
		}
		if(col != row) {
			if(r[ind] > 0) {
				pos = true;
			} else if (r[ind] < 0) {
				neg = true;
			}
		}
		if(col == lastcol) {
			if(pos ^ neg) { /*XOR - one or the other */
				result.emplace_back(row, m);
			}
		}
		ind++;
		col++;
		if(col == maxcol) {
			col = 0;
			row++;
		}
	}
	return result;
}
void SSMove::move(const Applicable& app, IntMatrix& result) const {
	const int* m = app.matrix_->data();
	int* r = result.data();
	int row = 0;
	int col = 0;
	int maxcol = app.matrix_->num_cols();
	int ind = 0;
	int max = app.matrix_->num_rows() * app.matrix_->num_cols();
	int match = app.row_;
	while(ind < max) {
		if(row == match || col == match) {
			r[ind] = -1 * m[ind];
		} else {
			r[ind] = m[ind];
		}
		ind++;
		col++;
		if(col == maxcol) {
			col = 0;
			row++;
		}
	}
	result.reset();
}
}

