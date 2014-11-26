/**
 * mmi_move.cc
 */
#include "mmi_move.h"

#include <iostream>
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
			conn_.insert(std::make_pair(conn[i], req[i]));
		}
	}
	void MMIMove::move(const Applicable& app, IntMatrix& result){
		const int* m = app.matrix_->data();
		int* r = result.data();
		const std::vector<int>& perm(*app.perm_);

		int ind = 0;
		int row = 0;
		int col = 0;
		bool srow = false;
		int subRow = -1;
		while(ind < app.matrix_->num_rows() * app.matrix_->num_cols()) {
			if(col == 0) {
				srow = false;
				for(size_t i = 0; i < app.submatrix_->size(); i++) {
					if(row == (*app.submatrix_)[i]) {
						srow = true;
						subRow = i;
						break;
					}
				}
			}
			if(srow) {
				bool inCol = false;
				int subCol;
				for(size_t i = 0; i < app.submatrix_->size(); i++) {
					if(col == (*app.submatrix_)[i]) {
						inCol = true;
						subCol = i;
						break;
					}
				}
				if(inCol) {
					/* In submatrix */
					r[ind] = app.match_.get(perm[subRow], perm[subCol]);
				} else {
					r[ind] = m[ind];
				}
			} else {
				r[ind] = m[ind];
			}
			ind++;
			col++;
			if(col == app.matrix_->num_cols()) {
				++row;
				col = 0;
			}
		}
		result.reset();
	}
	std::vector<MMIMove::Applicable> MMIMove::applicable_submatrices(
			const IntMatrix& matrix){
		MatrixPtr mp = std::make_shared<IntMatrix>(matrix);
		return applicable_submatrices(mp);
	}
	std::vector<MMIMove::Applicable> MMIMove::applicable_submatrices(
			const MatrixPtr matrix){
		std::vector<Applicable> result;
		if(matrix->num_cols() >= size_ && matrix->num_rows() >= size_) {
			SizedSubmatrixIterator iter(size_, *matrix);
			EquivQuiverMatrix m(size_, size_);
			while(iter.has_next()) {
				iter.next(m);
				bool equal = false;
				bool a = false;
				std::vector<int> perm;
				if(mata_.equals(m)) {
					perm = m.get_permutation(mata_);
					std::cout << m << std::endl;
					std::cout << "Perm: ";
					for(size_t i = 0; i < perm.size(); i++) std::cout << i << "->" << perm[i] << "  ";
					std::cout << std::endl;
					a = true;
					equal = true;
				} else if(matb_.equals(m)) {
					perm = m.get_permutation(matb_);
					equal = true;
				}
				if(equal) {
					std::vector<int> sub = iter.get_rows();
					std::cout << "Sub: ";
					for(size_t i = 0; i < sub.size(); i++) std::cout << i << "->" << sub[i] << "  ";
					std::cout << std::endl;
					mmi_conn::Submatrix s(matrix, std::move(sub), std::move(perm));
					std::cout << "Checking connections" << std::endl;
					if(check_connections(s)) {
						std::cout << "Connections match" << std::endl;
						if(a) {
							result.emplace_back(s, matb_);
						} else {
							result.emplace_back(s, mata_);
						}
					}
				}
			}
		}
		return std::move(result);
	}
	bool MMIMove::check_connections(const mmi_conn::Submatrix& submatrix) const {
		bool valid = true;
		/* i is the index of the row in the submatrix, s the index of the row in the
		 * main matrix. */
		for(int i = 0; i < size_ && valid; i++) {
			std::cout << i << std::endl;
			int p = (*submatrix.perm_)[i];
			if(conn_.find(p) != conn_.end()) {
				/* Row is one of the connections.
				 * Check that the connection requirements for this connection are
				 * satisfied. */
				std::cout << "Checking conn at " << p << " with connection " << (*submatrix.submatrix_)[i] << std::endl;
				valid = conn_.at(p)(submatrix, (*submatrix.submatrix_)[i]);
				continue;
			}
			int s = (*submatrix.submatrix_)[i];
			auto row = submatrix.matrix_->get_row(s);
			for(size_t j = 0; j < row.size(); j ++) {
				/** Need to check whether each entry in the row is non-zero only when
				 * it is inside the submatrix. */
				if(std::find(submatrix.submatrix_->begin(),
							submatrix.submatrix_->end(), j) == submatrix.submatrix_->end()
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
		bool result = true;
		seen_.resize(sub.matrix_->num_cols());
		seen_[0] = connection;
		for(int i = 0; i < sub.matrix_->num_cols() && result; i++) {
			if(std::find(sub.submatrix_->begin(), sub.submatrix_->end(), i)
					!= sub.submatrix_->end()) {
				/* i is in submatrix */
				continue;
			}
			if(sub.matrix_->get(connection, i) != 0) {
				seen_[1] = i;
				result = isUnconnected(sub, 2, i);
			}
		}
		return result;
	}
	bool Unconnected::isUnconnected(const Submatrix& sub, int size, int next) {
		bool result = true;
		for(int i = 0; i < sub.matrix_->num_cols() && result; i++) {
			if(std::find(seen_.begin(), seen_.begin() + size, i) != seen_.begin() + size) {
				/* i has been considered before */
				continue;
			}
			if(std::find(sub.submatrix_->begin(), sub.submatrix_->end(), i)
					!= sub.submatrix_->end()) {
				/* i is in submatrix */
				if(sub.matrix_->get(next, i) != 0) {
					result = false;
				}
				continue;
			}
			if(sub.matrix_->get(next, i) != 0) {
				seen_[size] = i;
				result = isUnconnected(sub, size + 1, i);
			}
		}
		return result;
	}
	bool Line::operator()(const Submatrix& sub, int connection) const {
		int next = -1;
		for(int i = 0; i < sub.matrix_->num_cols(); i++) {
			if(std::find(sub.submatrix_->begin(), sub.submatrix_->end(), i)
					!= sub.submatrix_->end()) {
				/* i is inside the submatrix. */
				continue;
			}
			if(sub.matrix_->get(connection, i) != 0) {
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
		for(int i = 0; i < sub.matrix_->num_cols(); i++) {
			if(i == prev) {
				continue;
			}
			int val = sub.matrix_->get(next, i);
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
		std::cout << "Running ConnectedTo for " << conn_[0] << std::endl;
					std::cout << "Perm: ";
					for(size_t i = 0; i < (*sub.perm_).size(); i++) std::cout << i << "->" << (*sub.perm_)[i] << "  ";
					std::cout << std::endl;
		for(size_t i = 0; i < conn_.size(); ++i) {
			for(size_t j = 0; j < sub.perm_->size(); ++j) {
				if((*sub.perm_)[j] == conn_[i]) {
					sub_conn_[i] = (*sub.submatrix_)[j];
					break;
				}
			}
		}
		for(size_t i = 0; i < sub_conn_.size(); ++i) {
			std::cout << "Subconn_ " << i << "->" << sub_conn_[i] << std::endl;
		}
		bool result = true;
		seen_.resize(sub.matrix_->num_cols());
		seen_[0] = connection;
		for(int i = 0; i < sub.matrix_->num_cols() && result; i++) {
			if(std::find(sub.submatrix_->begin(), sub.submatrix_->end(), i)
					!= sub.submatrix_->end()) {
				/* i is in submatrix */
				continue;
			}
			if(sub.matrix_->get(connection, i) != 0) {
				seen_[1] = i;
				result = isConnected(sub, 2, i);
			}
		}
		return result;
	}
	bool ConnectedTo::isConnected(const Submatrix& sub, int size, int next) {
		std::cout << "Next: "<< next<< std::endl;
		bool result = true;
		for(int i = 0; i < sub.matrix_->num_cols() && result; i++) {
			if(std::find(seen_.begin(), seen_.begin() + size, i) != seen_.begin() + size) {
				/* i has been considered before */
				continue;
			}
			if(std::find(sub.submatrix_->begin(), sub.submatrix_->end(), i)
					!= sub.submatrix_->end()) {
				/* i is in submatrix */
				if(sub.matrix_->get(next, i) != 0 &&
						std::find(sub_conn_.begin(), sub_conn_.end(), i) == conn_.end()) {
					std::cout << "Comes back to sub at " << i <<  std::endl;
					/* Comes back to submatrix in not allowed connection. */
					result = false;
				}
				continue;
			}
			if(sub.matrix_->get(next, i) != 0) {
				seen_[size] = i;
				std::cout << "Seen: ";
				for(int k = 0; k <=size; k++) std::cout << seen_[k] << ", ";
				std::cout << std::endl;
				result = isConnected(sub, size + 1, i);
			}
		}
		return result;
	}
	bool LineTo::operator()(const Submatrix& sub, int connection) {
		int next = -1;
		for(size_t i = 0; i < sub.perm_->size(); ++i) {
			if((*sub.perm_)[i] == conn_) {
				sub_conn_ = (*sub.submatrix_)[i];
				break;
			}
		}
		std::cout << "Checking lineto "<< conn_ << "->" << sub_conn_ << " at " << connection<< std::endl;
		for(int i = 0; i < sub.matrix_->num_cols(); i++) {
			if(std::find(sub.submatrix_->begin(), sub.submatrix_->end(), i)
					!= sub.submatrix_->end()) {
				/* i is inside the submatrix. */
				continue;
			}
			if(sub.matrix_->get(connection, i) != 0) {
				if(next != -1) {
					/* Have two arrows outside the submatrix, so not line. */
					std::cout << "Two arrows: " << next << " and "<< i << std::endl;
					return false;
				}
				next = i;
			}
		}
		if(next == -1) {
			/* No arrows out of connection. */
			std::cout << "No arrows" << std::endl;
			return false;
		}
		/* Don't need to check whether (next == conn_) as conn_ will be in the
		 * submatrix, so would be skipped. */
		return isLine(sub, next, connection);
	}
	bool LineTo::isLine(const Submatrix& sub, int next, int prev) const {
		std::cout << "Checking subline " << next << std::endl;
		int n = -1;
		for(int i = 0; i < sub.matrix_->num_cols(); i++) {
			if(i == prev) {
				continue;
			}
			if(sub.matrix_->get(next, i) != 0) {
				if(n != -1) {
					return false;
				}
				n = i;
			}
		}
		if(n == -1) {
			return false;
		}
		if(n == sub_conn_) {
			/* Line connected back to required vertex. */
			return true;
		}
		if(std::find(sub.submatrix_->begin(), sub.submatrix_->end(), n)
				!= sub.submatrix_->end()) {
			/* Line connects to other submatrix vertex */
			return false;
		}
		return isLine(sub, n, next);
	}
}
}
