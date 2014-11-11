/*
 * equivalence_checker.h
 *
 * Contains the EquivalenceChecker class which checks whether two matrices are
 * equivalent up to permuting their rows and columns.
 */
#pragma once

#include <vector>
#include <utility>

#include "equiv_quiver_matrix.h"

namespace cluster {
	/* Forward declaration required. */
	class EquivQuiverMatrix;

	class EquivalenceChecker {
		private:
			typedef EquivQuiverMatrix M;
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
		bool are_equivalent(const M &lhs, const M &rhs);
		/** Get the mapping used in last equals check. */
		std::vector<int> last_row_map() { return last_row_map_; }
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
		 * Stores information about possible mappings between the matrices.
		 */
		class Mapping {
			public:
				Mapping(int size)
					: row_mappings(size),
						col_mappings(size) {}
				void reset() {
					for(std::size_t i = 0; i < row_mappings.size(); ++i) {
						row_mappings[i].resize(0);
						col_mappings[i].resize(0);
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

		/** Sotres permuted a matrix. */
		IntMatrix ap_;
		/** Stores permuted b matrix. */
		IntMatrix pb_;
		/** Size of matrices to check. */
		int size_;
		/** Info about the possible permutations between the specified matrices. */
		Mapping maps_;
		/** Mapping used in last equals check run. */
		std::vector<int> last_row_map_;
		std::vector<int> last_col_map_;

		/** Check whether the columns of the matrices match. */
		bool do_columns_match(const M& a, const M& b);
		/** Check whether the rows of the matrices match. */
		bool do_rows_match(const M& a, const M& b);
		/** Check whether the row/column sums match. */
		bool sums_equivalent(const M& a, const M& b) const;
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
				const M& a, const M& b);

	};
}

