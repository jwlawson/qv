#pragma once
#include "int_matrix.h"
#include <cstdlib>

namespace cluster {
	class QuiverMatrix :
		public IntMatrix {
	 public:
		QuiverMatrix();
		QuiverMatrix(const int rows, const int cols);
		QuiverMatrix(const int rows, const int cols, const int values[]);
		QuiverMatrix(const QuiverMatrix &mat);
		QuiverMatrix(IntMatrix matrix);
		QuiverMatrix(QuiverMatrix&&);
		~QuiverMatrix();
		bool is_infinite() const;
		QuiverMatrix &operator=(QuiverMatrix mat);

		template<class T>
		T mutate(const int k, T &result) const {
			
			int index = 0;
			std::vector<int> k_row(std::move(get_row(k)));
			std::vector<int> k_col(std::move(get_col(k)));
			for (int i = 0; i < num_rows(); i++) {
				if(i == k) {
					for(int j = 0; j < num_cols_; ++j) {
						result.data_[index] = -1 * data_[index];
						++index;
					}
				} else {
					for(int j = 0; j< num_cols_; ++j) {
						if(j==k) {
							result.data_[index] = -1 * data_[index];
						} else {
							result.data_[index] = data_[index] + (std::abs(k_col[i]) * k_row[j] 
									+ k_col[i] * std::abs(k_row[j])) / 2;
						}
						++index;
					}
				}
			}
			result.reset();
			return result;
		}
		template<class T>
		T subquiver(const int k, T& result) const {
			submatrix(k, k, result);
			int zero = result.zero_row();
			if (zero != -1) {
				T tmp(result.num_rows() - 1, result.num_cols() - 1);
				result.subquiver(zero, tmp);
				result=std::move(tmp);
			}
			result.reset();
			return result;
		}

	 private:
	};
}

namespace std {
	/* Add hash function to the std::hash struct. */
	template <>
	struct hash<cluster::QuiverMatrix> {
		size_t operator()(const cluster::QuiverMatrix &x) const {
			return x.hash();
		}
	};
	/* Add equals function to std::equal_to */
	template<>
	struct equal_to<cluster::QuiverMatrix> {
		bool operator()(const cluster::QuiverMatrix &lhs,
		                const cluster::QuiverMatrix &rhs) const {
			return lhs.equals(rhs);
		}
	};
}

