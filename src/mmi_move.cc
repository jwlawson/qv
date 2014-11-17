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
			conn_(){
		for(auto i = connections.begin(); i != connections.end(); ++i) {
			conn_.insert(std::make_pair(*i, mmi_conn::Unconnected()));
		}
	}
	MMIMove::MMIMove(const IntMatrix& mata, const IntMatrix& matb,
			const std::vector<int>& conn, const std::vector<ConnReq>& req)
		: mata_(mata), matb_(matb), size_(mata.num_rows()), conn_() {
		for(size_t i = 0; i < conn.size(); ++i) {
			conn_.insert(std::make_pair(i, req[i]));
		}
	}
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
					mmi_conn::Submatrix s(matrix, sub, perm);
					if(check_connections(s)) {
						result.push_back(sub);
					}
				}
			}
		}
		return result;
	}
	bool MMIMove::check_connections(const mmi_conn::Submatrix& submatrix) {
		bool valid = true;
		/* i is the index of the row in the submatrix, s the index of the row in the
		 * main matrix. */
		for(int i = 0; i < size_ && valid; i++) {
			int p = submatrix.perm_[i];
			if(conn_.find(p) != conn_.end()) {
				/* Row is one of the connections.
				 * Check that the connection requirements for this connection are
				 * satisfied. */
				valid = conn_[p](submatrix, i);
				continue;
			}
			int s = submatrix.submatrix_[i];
			auto row = submatrix.matrix_.get_row(s);
			for(size_t j = 0; j < row.size(); j ++) {
				/** Need to check whether each entry in the row is non-zero only when
				 * it is inside the submatrix. */
				if(std::find(submatrix.submatrix_.begin(),
							submatrix.submatrix_.end(), j) == submatrix.submatrix_.end()
						&& row[j] != 0) {
					valid = false;
					break;
				}
			}
		}
		return valid;
	}

namespace mmi_conn {
	bool Unconnected::operator()(const Submatrix& sub, int connection) {
		return true;
	}
	bool Line::operator()(const Submatrix& sub, int connection) const {
		int next = -1;
		for(int i = 0; i < sub.matrix_.num_cols(); i++) {
			if(std::find(sub.submatrix_.begin(), sub.submatrix_.end(), i)
					!= sub.submatrix_.end()) {
				/* i is inside the submatrix. */
				continue;
			}
			if(sub.matrix_.get(connection, i) != 0) {
				if(next != -1) {
					/* Have two arrows outside the submatrix, so not line. */
					return false;
				}
				next = i;
			}
		}
		if(next == -1) {
			/* No arrows out of connection. */
			return true;
		}
		return isLine(sub, next, connection);
	}
	bool Line::isLine(const Submatrix& sub, int next, int prev) const {
		int n = -1;
		for(int i = 0; i < sub.matrix_.num_cols(); i++) {
			if(i == prev) {
				continue;
			}
			int val = sub.matrix_.get(next, i);
			if(val != 0) {
				if(n != -1) {
					return false;
				}
				n = i;
			}
		}
		if(n == -1) {
			return true;
		}
		return isLine(sub, n, next);
	}
	bool ConnectedTo::operator()(const Submatrix& sub, int connection) {
		return false;
	}
	bool LineTo::operator()(const Submatrix& sub, int connection) {
		return false;
	}
}
}
