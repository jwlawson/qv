/**
 * mmi_move.cc
 */
#include "mmi_move.h"

#include <iostream>

#include "sized_submatrix_iterator.h"

namespace cluster {
	MMIMove::MMIMove(IntMatrix& mata, IntMatrix& matb, std::vector<int> connections)
		:	mata_(mata),
			matb_(matb),
			size_(mata.num_rows()),
			conn_(connections){}
	void MMIMove::move(IntMatrix& matrix, std::vector<int> submatrix){

	}
	std::vector<std::vector<int>> MMIMove::applicable_submatrices(IntMatrix& matrix){
		std::vector<std::vector<int>> result;
		if(matrix.num_cols() > size_ && matrix.num_rows() > size_) {
			SizedSubmatrixIterator iter(size_, matrix);
			while(iter.has_next()) {
				EquivQuiverMatrix m(iter.next());
				std::cout << m << std::endl;
				if(m.equals(mata_) || m.equals(matb_)) {
					std::vector<int> sub = iter.get_rows();
					if(check_connections(matrix, sub)) {
						result.push_back(sub);
					}
				}
			}
		}
		return result;
	}
	bool MMIMove::check_connections(IntMatrix& matrix, std::vector<int> submatrix) {
		bool valid = true;
		for(auto i = submatrix.begin(); i != submatrix.end() && valid; i++) {
			int s = *i;
			auto row = matrix.get_row(s);
			for(size_t j = 0; j < row.size(); j ++) {
				/** Need to check whether each entry in the row is non-zero only when
				 * it coincides with the correct connections.
				 * However because the submatrices are checked up to equivalence, the
				 * connections could be permuted around a bit. */
			}
		}
		return false;
	}
}
