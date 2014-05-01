#pragma once
#include "int_matrix.h"
#include <vector>
#include <utility>

namespace cluster {
	class EquivalenceChecker {
	 public:
		 /**
			* Get the singleton instance of EquivalenceChecker.
			* This is less important now that we are not creating every possible
			* permutation, but still reduces the amount of allocations.
			*/
		static std::shared_ptr<EquivalenceChecker> Get(const int size);

		/** Create empty checker that is essentially useless. */
		EquivalenceChecker();
		/**
		 * Create instance with pre-allocated vectors of the required size.
		 * @param size Size of matrices to be checked
		 */
		EquivalenceChecker(const int size);
		/**
		 * Check if the two matrices are equivalent up to a permutation of their
		 * rows and columns.
		 * @param lhs First matrix
		 * @param rhs Second matrix
		 * @return true if matrices are equivalent
		 */
		bool are_equivalent(const IntMatrix &lhs, const IntMatrix &rhs);
		/**
		 * Allocation operator.
		 */
		EquivalenceChecker& operator=(EquivalenceChecker mat);
		/**
		 * Swap function used in allocating. Swaps each of the private data members.
		 */
		friend void swap(EquivalenceChecker& f, EquivalenceChecker& s);

	 private:
		/** Cached singleton instance. */
		static std::weak_ptr<EquivalenceChecker> instance_;

		/**
		 * Class to hold the row/column sums and absolute value versions. Provides
		 * method to easily reset these.
		 */
		class Info {
			public:
				Info(int size)
					: rows(size),
						cols(size) {}
				void reset() {
					for(std::size_t i = 0; i < rows.size(); ++i) {
						rows[i].first = 0;
						rows[i].second = 0;
						cols[i].first = 0;
						cols[i].second = 0;
					}
				}
				friend void swap(Info& a, Info& b){
					using std::swap;
					swap(a.rows, b.rows);
					swap(a.cols, b.cols);
				}
				std::vector<std::pair<int, int>> rows;
				std::vector<std::pair<int, int>> cols;
		};

		/**
		 * Stores information about possible mappings between the matrices.
		 */
		class Mapping {
			public:
				Mapping(int size)
					: row_mappings(size),
						col_mappings(size) {}
				void reset() {
					for(std::size_t i = 0; i < row_mappings.size(); ++i) {
						row_mappings[i].clear();
						col_mappings[i].clear();
					}
				}
				void update_row_mapping(const int a_index, const int b_index){
					row_mappings[a_index].push_back(b_index);
				}
				void update_column_mapping(const int a_index, const int b_index){
					col_mappings[a_index].push_back(b_index);
				}
				friend void swap(Mapping& a, Mapping& b) {
					using std::swap;
					swap(a.row_mappings, b.row_mappings);
					swap(a.col_mappings, b.col_mappings);
				}

				std::vector<std::vector<int>> row_mappings;
				std::vector<std::vector<int>> col_mappings;
		};

		IntMatrix ap_;
		IntMatrix pb_;
		int size_;
		Info ai_;
		Info bi_;
		Mapping maps_;

		/** Fill the Info sums with the correct values for both a and b. */
		void calc_sums(const IntMatrix &a, const IntMatrix &b);
		/** Check whether the columns of the matrices match. */
		bool do_columns_match(const IntMatrix& a, const IntMatrix& b);
		/** Check whether the rows of the matrices match. */
		bool do_rows_match(const IntMatrix& a, const IntMatrix& b);
		/** Check whether the row/column sums match. */
		bool sums_equivalent() const;
		/** 
		 * Convenience method to check if two int vectors contain the same numbers
		 * in different orders.
		 */
		bool arrays_equivalent(const std::vector<int>& a,
				const std::vector<int>& b) const;
		/**
		 * Check whether the two matrices are permutations by considering the
		 * mappings calculated earlier. Uses recursion on the index.
		 */
		bool check_perm(std::vector<int>& row_map, int index, 
				const IntMatrix& a, const IntMatrix& b);

	};
}

