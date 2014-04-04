#pragma once
#include "quiver_matrix.h"
#include "equivalence_checker.h"

namespace cluster {
	class EquivQuiverMatrix :
		public QuiverMatrix {
	 public:
		EquivQuiverMatrix();
		EquivQuiverMatrix(const int rows, const int cols);
		EquivQuiverMatrix(const int rows, const int cols, const int values[]);
		EquivQuiverMatrix(IntMatrix);
		EquivQuiverMatrix(EquivQuiverMatrix&&);
		virtual bool equals(const IntMatrix &mat) const;

	 private:
		std::shared_ptr<EquivalenceChecker> checker_;
		EquivQuiverMatrix &operator=(EquivQuiverMatrix mat);

		virtual std::size_t compute_hash() const;
	};
}

namespace std {
	/* Add hash function to the std::hash struct. */
	template <>
	struct hash<cluster::EquivQuiverMatrix> {
		size_t operator()(const cluster::EquivQuiverMatrix &x) const {
			return x.hash();
		}
	};
	/* Add equals function to std::equal_to */
	template<>
	struct equal_to<cluster::EquivQuiverMatrix> {
		bool operator()(const cluster::EquivQuiverMatrix &lhs,
		                const cluster::EquivQuiverMatrix &rhs) const {
			return lhs.equals(rhs);
		}
	};
}

