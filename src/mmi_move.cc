/**
 * mmi_move.cc
 */
#include "mmi_move.h"

#include <algorithm>

#include "sized_submatrix_iterator.h"

namespace cluster {
	MMIMove::MMIMove(const IntMatrix& mata, const IntMatrix& matb, const
			std::vector<int>& connections)
		:	mata_(mata),
			matb_(matb),
			size_(mata.num_rows()),
			conn_(connections){}
	void MMIMove::move(const IntMatrix& matrix, const std::vector<int>&
			submatrix){

	}
	std::vector<std::vector<int>> MMIMove::applicable_submatrices(const IntMatrix&
			matrix){
		std::vector<std::vector<int>> result;
		if(matrix.num_cols() >= size_ && matrix.num_rows() >= size_) {
			SizedSubmatrixIterator iter(size_, matrix);
			while(iter.has_next()) {
				EquivQuiverMatrix m(iter.next());
				bool equal = false;
				std::vector<int> perm;
				if(m.equals(mata_)) {
					perm = m.get_permutation(mata_);
					equal = true;
				} else if(m.equals(matb_)) {
					perm = m.get_permutation(matb_);
					equal = true;
				}
				if(equal) {
					std::vector<int> sub = iter.get_rows();
					if(check_connections(matrix, sub, perm)) {
						result.push_back(sub);
					}
				}
			}
		}
		return result;
	}
	bool MMIMove::check_connections(const IntMatrix& matrix, const
			std::vector<int>& submatrix, const std::vector<int>& perm) {
		bool valid = true;
		/* i is the index of the row in the submatrix, s the index of the row in the
		 * main matrix. */
		for(int i = 0; i < size_ && valid; i++) {
			int p = perm[i];
			if(std::find(conn_.begin(), conn_.end(), p) != conn_.end()) {
				/* Row is one of the connections, so can be anything.*/
				continue;
			}
			int s = submatrix[i];
			auto row = matrix.get_row(s);
			for(size_t j = 0; j < row.size(); j ++) {
				/** Need to check whether each entry in the row is non-zero only when
				 * it is inside the submatrix. */
				if(std::find(submatrix.begin(), submatrix.end(), j) == submatrix.end()
						&& row[j] != 0) {
					valid = false;
					break;
				}
			}
		}
		return valid;
	}
}
