/**
 * move_class_loader.cc
 */
#include "move_class_loader.h"

namespace cluster {
	MoveClassLoader::MoveClassLoader(const MPtr matrix,
			const std::vector<MovePtr>& moves)
		: last_(),
			size_(matrix->num_rows()),
			moves_(moves), queue_(),
			map_(),
			depth_(0) {
		queue_.push_back(matrix);
		map_[matrix] = std::make_pair(0, nullptr);
	}
	bool MoveClassLoader::has_next(){
		return !queue_.empty();
	}
	std::shared_ptr<IntMatrix> MoveClassLoader::next(){
		typedef std::vector<MMIMove::Applicable> AVec;
		MPtr result = queue_.front();
		queue_.pop_front();
		depth_ = map_[result].first;
		for(auto i = moves_.begin(); i != moves_.end(); ++i) {
			MovePtr m = *i;
			if(m != map_[result].second) {
				AVec appl = m->applicable_submatrices(result);
				for(size_t j = 0; j < appl.size(); ++j) {
					MPtr new_matrix = std::make_shared<M>(size_, size_); 
					m->move(appl[j], *new_matrix);
					if(map_.find(new_matrix) == map_.end()) {
						map_[new_matrix].first = depth_ + 1;
						map_[new_matrix].second = m;
						queue_.push_back(new_matrix);
					}
				}
			}
		}
		last_ = result;
		return result;
	}
	int MoveClassLoader::depth(){
		return depth_;
	}
}

