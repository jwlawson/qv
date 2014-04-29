/*
 * cycle_info.cc
 */
#include "cycle_info.h"

namespace cluster {
	
	CycleInfo::CycleInfo() : cycles_(), edges_(), size_() {}

	CycleInfo::CycleInfo(const QuiverMatrix& matrix)
		:	cycles_(),
			edges_(),
			size_(std::min(matrix.num_rows(), matrix.num_cols())) {
		comp_cycles(matrix);
		find_double_edges(matrix);
	}

	bool CycleInfo::equals(const CycleInfo& rhs) const {
		if(cycles_.size() != rhs.cycles_.size()){
			return false;
		}
		if(edges_.size() != rhs.edges_.size()) {
			return false;
		}
		if(cycles_ == rhs.cycles_ && edges_ == rhs.edges_) {
			return true;
		}
		std::vector<int> dedge_start;
		std::vector<int> dedge_end;
		for(const DEdge& edge : rhs.edges_) {
			dedge_start.push_back(edge.first);
			dedge_end.push_back(edge.second);
		}
		std::vector<std::vector<int>> edge_maps(rhs.size_);
		for(const DEdge& edge : edges_) {
			for(int i : dedge_start) {
				edge_maps[i].push_back(edge.first);
			}
			for(int i : dedge_end) {
				edge_maps[i].push_back(edge.second);
			}
		}
		std::vector<int> count(size_);
		for(auto& cycle : cycles_) {
			for(int i = 0; i < size_; ++i) {
				if(cycle.contains(i)){
					count[i]++;
				}
			}
		}

		std::vector<int> r_count(rhs.size_);
		for(auto& cycle : rhs.cycles_) {
			for(int i = 0; i < rhs.size_; ++i) {
				if(cycle.contains(i)){
					r_count[i]++;
				}
			}
		}
		std::vector<std::vector<int>> cycle_maps(rhs.size_);
		for(int i = 0; i < rhs.size_; ++i) {
			for(int j = 0; j < size_; ++j) {
				if(r_count[i] == count[j]) {
					cycle_maps[i].push_back(j);
				}
			}
		}
		std::vector<std::vector<int>> maps(rhs.size_);
		for(int i = 0; i < rhs.size_; ++i) {
			if(edge_maps[i].empty()){
				maps[i] = cycle_maps[i];
			} else {
				for(int edge : edge_maps[i]) {
					for(int cycle : cycle_maps[i]) {
						if(edge == cycle) {
							maps[i].push_back(edge);
						}
					}
				}
			}
		}
		std::vector<int> mapping(rhs.size_);
		return perm_equals(maps, rhs, mapping, 0);
	}

	void CycleInfo::comp_cycles(const QuiverMatrix& matrix) {
		std::vector<int> cycles;
		cycles.reserve(matrix.num_cols());
		for(int i = 0; i < matrix.num_rows(); ++i) {
			cycles_from(i, cycles, 0, matrix);
		}
	}

	void CycleInfo::find_double_edges(const QuiverMatrix& matrix) {
		const int* data = matrix.data();
		for(int i = 0; i < matrix.num_rows() * matrix.num_cols(); ++i) {
			if(data[i] == 2) {
				int col = i % matrix.num_cols();
				int row = (i - col)/matrix.num_cols();
				DEdge dedge = std::make_pair(row, col);
				for(auto& cycle : cycles_){
					if(cycle.contains(dedge)) {
						edges_.insert(dedge);
						break;
					}
				}
			}
		}
	}

	/*
	 * This iterates through all possible cycles, so will look at each cycle many
	 * times. This could probably be optimized, but you have to be careful to
	 * ensure that *all* cycles will be found, as some times you will have two
	 * cycles which share multiple vertices.
	 *
	 * The recursion with the same vector means that the contains method needs to
	 * know which index to compare up to, as later values could have been set by
	 * previous method calls. Also the vector.reserve and then vector[i] = *
	 * combination means that the vector size is never actually updated to reflect
	 * how many elements have been assigned, so this method cannot be used, nor
	 * can vector.empty.
	 */
	void CycleInfo::cycles_from(int vertex, std::vector<int>& cycle, int index,
			const QuiverMatrix& matrix) {
		if(index != 0 && vertex == cycle[0]) {
			/* Loop complete, so insert into set. Note that if the cycle has been
			 * found before the suplicate will not be inserted, which is exactly the
			 * behaviour we want. */
			Cycle add(cycle, index);
			cycles_.insert(std::move(add));
			return;
		} else if(vector_contains(cycle, vertex, index)
				|| index > (int)cycle.capacity() ) {
			/* Got back to already seen vertex, so not a loop, or gone round too many
			 * times, so the index is larger than the matrix. Note this should not
			 * happen. Also note that the cast above is OK, as we will never be
			 * dealing with matrices with more than MAX_INT columns.*/
			return;
		}
		cycle[index] = vertex;
		index++;
		std::vector<int> row = matrix.get_row(vertex);
		for(std::size_t i = 0; i < row.size(); ++i) {
			if(row[i] > 0) {
				cycles_from(i, cycle, index, matrix);
			}
		}
	}

	bool CycleInfo::vector_contains(const std::vector<int>& vec,
			const int value, const int size) {
		for(int i = 0; i < size; ++i) {
			if(vec[i] == value){
				return true;
			}
		}
		return false;
	}

	bool CycleInfo::perm_equals(const std::vector<std::vector<int>>& maps,
			const CycleInfo& rhs, std::vector<int>& so_far, int index) const {
		if(index == rhs.size_) {
			/* Mapping complete. */
			bool result = true;
			for(auto& cycle : cycles_){
				bool eq = false;
				for(auto& rc : rhs.cycles_) {
					if(cycle.equals(rc, so_far)) {
						eq = true;
						break;
					}
				}
				if(!eq) {
					result = false;
					break;
				}
			}
			return result;
		}
		if(maps[index].empty()){
			so_far[index] = -1;
			return perm_equals(maps, rhs, so_far, index + 1);
		}
		bool result = false;
		for(std::size_t i = 0; i < maps[index].size() && !result; ++i) {
			bool contains = false;
			for(int j = 0; j < index; ++j) {
				if(maps[index][i] == so_far[j]){
					contains = true;
				}
			}
			if(!contains){
				so_far[index] = maps[index][i];
				result = perm_equals(maps, rhs, so_far, index + 1);
			}
		}
		return result;
	}


}

