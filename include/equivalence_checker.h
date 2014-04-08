#pragma once
#include "int_matrix.h"
#include <vector>

namespace cluster {
	class EquivalenceChecker {
	 public:
		static std::shared_ptr<EquivalenceChecker> Get(const int size);

		EquivalenceChecker();
		EquivalenceChecker(const int size);
		bool are_equivalent(const IntMatrix &lhs, const IntMatrix &rhs);
		EquivalenceChecker& operator=(EquivalenceChecker mat);
		friend void swap(EquivalenceChecker& f, EquivalenceChecker& s);

	 private:
		static std::weak_ptr<EquivalenceChecker> instance_;

		class Info {
			public:
				Info(int size)
					: row_sum(size),
						abs_row_sum(size),
						col_sum(size),
						abs_col_sum(size) {}
				void reset() {
					for(uint i = 0; i < row_sum.size(); ++i) {
						row_sum[i] = 0;
						abs_row_sum[i] = 0;
						col_sum[i] = 0;
						abs_col_sum[i] = 0;
					}
				}
				std::vector<int> row_sum;
				std::vector<int> abs_row_sum;
				std::vector<int> col_sum;
				std::vector<int> abs_col_sum;
		};

		class Mapping {
			public:
				Mapping(int size)
					: row_mappings(size),
						col_mappings(size) {}
				void reset() {
					for(uint i = 0; i < row_mappings.size(); ++i) {
						row_mappings[i] = 57;
						col_mappings[i] = 57;
					}
				}
				std::vector<int> row_mappings;
				std::vector<int> col_mappings;
		};

		std::vector<IntMatrix> permutations_;
		IntMatrix ap_;
		IntMatrix pb_;
		int size_;
		Info ai_;
		Info bi_;
		Mapping maps_;

		bool perm_values(const int size, const int i, int *values) const;
		int factorial(const int num) const;
		void calc_sums(const IntMatrix &a, const IntMatrix &b);
		bool do_columns_match(const IntMatrix& a, const IntMatrix& b);
		bool do_rows_match(const IntMatrix& a, const IntMatrix& b);
		void update_row_mapping(const int a_index, const int b_index);
		void update_column_mapping(const int a_index, const int b_index);
		bool permutation_valid(const IntMatrix& perm) const;
		bool perm_column_valid(int mapping,
		                       const IntMatrix& perm,
		                       const int index)const;
		bool perm_row_valid(int mapping,
		                       const IntMatrix& perm,
		                       const int index)const;
		bool sums_equivalent() const;
		bool arrays_equivalent(std::vector<int> a, std::vector<int> b) const;

	};
}
