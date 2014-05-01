/*
 * array_utils.h
 */
#pragma once

#include <vector>

namespace cluster {
	namespace arrays {
		int *sort(int *arr, const int size);
		std::size_t hash(const int *arr, const int size);
		std::size_t hash(const std::vector<int>& arr);
		std::size_t hash(const std::vector<std::pair<int, int>>& vec);
		/** Find the number of occurences of the value in the vector. */
		int number_in(const std::vector<int>& arr, const int val);

		/**
		 * Find the next index at which the specified value is found in the vector,
		 * after the previous index.
		 */
		template<class T>
		int next_index_of(const std::vector<T>& arr, const T& val, const std::size_t prev){
			for (std::size_t i = prev + 1; i < arr.size(); i++) {
				if (arr[i] == val) {
					return i;
				}
			}
			return -1;
		}
		/**
		 * Check if two vectors are equal.
		 *
		 * PROBABLY NOT NEEDED, use a == b.
		 */
		bool equal(const std::vector<int>& a, const std::vector<int>& b);
	}
}
