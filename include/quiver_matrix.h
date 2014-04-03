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
			for (int i = 0; i < num_rows(); i++) {
				for (int j = 0; j < num_cols(); j++) {
					int a;
					if (i == k || j == k) {
						a = -1 * get(i, j);
					} else {
						a =	get(i, j) + (std::abs(get(i, k)) * get(k, j) + get(i, k)
						                 * std::abs(get(k, j))) / 2;
					}
					result.set(i, j, a);
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
				auto tmp = T(result.num_rows() - 1, result.num_cols() - 1);
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

