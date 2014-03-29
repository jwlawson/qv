#pragma once
#include "quiver_matrix.h"
#include "equivalence_checker.h"

namespace cluster{
	class EquivQuiverMatrix :
		public QuiverMatrix
	{
	public:
		EquivQuiverMatrix(const int rows, const int cols);
		EquivQuiverMatrix(const int rows, const int cols, const int values[]);
		~EquivQuiverMatrix();
		virtual const bool equals(const IntMatrix& mat) const;

	private:
		std::shared_ptr<EquivalenceChecker> checker_;

		virtual const std::size_t compute_hash() const;
	};
}
