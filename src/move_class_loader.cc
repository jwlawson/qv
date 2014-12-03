/**
 * move_class_loader.cc
 */
#include "move_class_loader.h"

namespace cluster {
	MoveClassLoader::MoveClassLoader(const MPtr matrix,
			const std::vector<MovePtr>& moves,
			bool sinksource)
		:	size_(matrix->num_rows()),
			moves_(moves),
			ssmove_(),
			queue_({matrix}),
			map_({ {matrix, Depth()} }),
			depth_(),
			sinksource_(sinksource) {}
	std::shared_ptr<EquivQuiverMatrix> MoveClassLoader::next(){
		typedef MMIMove::Applicable MMIApp;
		typedef std::vector<MMIApp> MMIAppVec;
		typedef SSMove::Applicable SSApp;
		typedef std::vector<SSApp> SSAppVec;
		MPtr result = queue_.front();
		queue_.pop_front();
		depth_ = map_[result];
		if(sinksource_) {
			SSAppVec ssapp = ssmove_.applicable_submatrices(result);
			for(auto app = ssapp.begin(); app != ssapp.end(); ++app) {
				MPtr ss_new = std::make_shared<M>(size_, size_);
				ssmove_.move(*app,*ss_new);
				if(map_.find(ss_new) == map_.end()) {
					map_[ss_new] = Depth(depth_.moves_, depth_.sinksource_ + 1);
					queue_.push_back(ss_new);
				}
			}
		}
		for(auto i = moves_.begin(); i != moves_.end(); ++i) {
			MovePtr m = *i;
			MMIAppVec appl = m->applicable_submatrices(result);
			for(size_t j = 0; j < appl.size(); ++j) {
				MPtr new_matrix = std::make_shared<M>(size_, size_); 
				m->move(appl[j], *new_matrix);
				if(map_.find(new_matrix) == map_.end()) {
					map_[new_matrix] = Depth(depth_.moves_ + 1, depth_.sinksource_);
					queue_.push_back(new_matrix);
				}
			}
		}
		return result;
	}
}

