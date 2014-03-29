#pragma once
#include "int_matrix.h"
#include <cstdlib>

namespace cluster
{
	class QuiverMatrix :
		public IntMatrix
	{
	public:
		QuiverMatrix(const int rows, const int cols);
		QuiverMatrix(const int rows, const int cols, const int values[]);
		~QuiverMatrix();
		const bool is_infinite() const;
		
		template<class T> 
		T mutate(const int k, T& result) const
		{
			for (int i = 0; i < num_rows(); i++) {
				for (int j = 0; j < num_cols(); j++) {
					int a;
					if (i == k || j == k) {
						a = -1 * get(i, j);
					}
					else {
						a =	get(i, j) + (std::abs(get(i, k)) * get(k, j) + get(i, k)
							* std::abs(get(k, j))) / 2;
					}
					result.set(i, j, a);
				}
			}
			return result;
		}
		template<class T> 
		std::shared_ptr<T> subquiver(const int k, std::shared_ptr<T> result) const
		{
			result = submatrix(k, k, result);
			int zero = result.zero_row();
			if (zero != -1) {
				auto tmp = std::make_shared<T>(result.num_rows(), result.num_cols());
				tmp = result.subquiver(zero, tmp);
				// Should decrement reference number for the old result
				result = tmp;
			}
			return result;
		}

	private:
	};
}
