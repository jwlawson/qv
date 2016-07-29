/*
 * array_utils.h
 * Copyright 2014-2015 John Lawson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 */
#pragma once

#include <algorithm>
#include <vector>

namespace cluster {
	namespace arrays {
		int *sort(int *arr, const int size);
		std::size_t hash(const int *arr, const int size);
		template<class T>
		std::size_t hash(const std::vector<T>& arr);
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
	template<class T>
	inline
	std::size_t hash(const std::vector<T>& vec) {
		T const * data_ = vec.data();
		std::size_t hash{113};
		for (int_fast16_t i = 0; i < vec.size(); i++) {
			hash *= 31;
			hash += data_[i];
		}
		return hash;
	}
}
