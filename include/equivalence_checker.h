#pragma once
#include "int_matrix.h"
#include <vector>

namespace cluster {
	class EquivalenceChecker {
	 public:
		static std::shared_ptr<EquivalenceChecker> Get(const int size);

		EquivalenceChecker();
		EquivalenceChecker(const int size);
		~EquivalenceChecker();
		const bool are_equivalent(const IntMatrix &lhs, const IntMatrix &rhs);
	 private:
		static std::weak_ptr<EquivalenceChecker> instance_;

		int size_;
		std::vector<IntMatrix> permutations_;
		IntMatrix ap_;
		IntMatrix pb_;

		bool perm_values(const int size, const int i, int *values) const;
		int factorial(const int num) const;
		std::vector<int> new_mapping_array(const int size) const;
		bool do_columns_match(const IntMatrix a, const IntMatrix b, const int a_cols,
		                      const std::vector<int> a_col_sum, const std::vector<int> a_abs_col_sum,
		                      const std::vector<int> b_col_sum, const std::vector<int> b_abs_col_sum,
		                      std::vector<int> &col_mappings) const;
		bool do_rows_match(const IntMatrix a, const IntMatrix b, const int a_rows,
		                   const std::vector<int> a_row_sum, const std::vector<int> a_abs_row_sum,
		                   const std::vector<int> b_row_sum, const std::vector<int> b_abs_row_sum,
		                   std::vector<int> &row_mappings) const;
		void update_mapping(const int num_rows, std::vector<int> &mappings,
		                    const int a_index,
		                    const int b_index) const;
		bool permutation_valid(const int a_rows, const int a_cols,
		                       std::vector<int> col_mappings,
		                       const IntMatrix perm) const;
		bool perm_column_valid(const int num, std::vector<int> mappings,
		                       const IntMatrix perm,
		                       const int index)const;
		bool arrays_equivalent(std::vector<int> a, std::vector<int> b) const;
	};
}
