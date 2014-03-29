#include "array_utils.h"
#include <algorithm>

namespace cluster{
	namespace arrays{
		int* sort(int* arr, const int size)
		{
			std::sort(arr, arr + size);
			return arr;
		}
		const std::size_t hash(const int* arr, const int size)
		{
			std::size_t hash = 47;
			for (int i = 0; i < size; i++){
				hash *= 31;
				hash += arr[i];
			}
			return hash;
		}
		const std::size_t hash(const std::vector<int> arr)
		{
			std::size_t hash = 47;
			for (uint i = 0; i < arr.size(); i++){
				hash *= 31;
				hash += arr[i];
			}
			return hash;
		}
		int number_in(const std::vector<int> arr, const int val)
		{
			int count = 0;
			for (uint i = 0; i < arr.size(); i++) {
				if (arr[i] == val) {
					count++;
				}
			}
			return count;
		}
		int next_index_of(const std::vector<int> arr, const int val, const int prev)
		{
			for (uint i = prev + 1; i < arr.size(); i++) {
				if (arr[i] == val) {
					return i;
				}
			}
			return -1;
		}
		bool equal(const std::vector<int> a, const std::vector<int> b)
		{
			if (a.size() != b.size()){
				return false;
			}
			for (uint i = 0; i < a.size(); i++){
				if (a[i] != b[i]){
					return false;
				}
			}
			return true;
		}
	}
}