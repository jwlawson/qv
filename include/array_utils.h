#pragma once
#include <cstddef>
#include <vector>

namespace cluster {
	namespace arrays {
		int *sort(int *arr, const int size);
		std::size_t hash(const int *arr, const int size);
		std::size_t hash(const std::vector<int>& arr);
		std::size_t hash(const std::vector<std::pair<int, int>>& vec);
		int number_in(const std::vector<int>& arr, const int val);
		int next_index_of(const std::vector<int>& arr, const int val, const int prev);
		bool equal(const std::vector<int>& a, const std::vector<int>& b);
	}
}
