/*
 * cycle.cc
 */

#include "cycle.h"

namespace cluster {

	/* Default ctr does nothing really. */
	Cycle::Cycle() : cycle_() {}

	Cycle::Cycle(const std::vector<int>& vec) : cycle_() {
		using std::size_t;
		cycle_.reserve(vec.size());
		size_t index = smallest_index(vec);
		for(size_t i = index; i < vec.size(); ++i) {
			cycle_.push_back(vec[i]);
		}
		for(size_t i = 0; i < index; ++i) {
			cycle_.push_back(vec[i]);
		}
	}
	
	bool Cycle::equals(const Cycle& rhs) {
		return cycle_ == rhs.cycle_;
	}

	bool Cycle::contains(const int value) {
		using std::size_t;
		for(size_t i = 0; i < cycle_.size(); ++i) {
			if(cycle_[i] == value) {
				return true;
			}
		}
		return false;
	}

	std::size_t Cycle::smallest_index(const std::vector<int>& vec) {
		using std::size_t;
		int smallest = vec[0];
		size_t index = 0;
		for(size_t i = 1; i < vec.size(); ++i) {
			if(vec[i] < smallest) {
				smallest = vec[i];
				index = i;
			}
		}
		return index;
	}

}

