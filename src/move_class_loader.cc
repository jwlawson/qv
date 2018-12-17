/**
 * move_class_loader.cc
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
#include "move_class_loader.h"

namespace cluster {

MoveClassLoader::MoveClassLoader(const MPtr matrix,
                                 const std::vector<MovePtr>& moves,
                                 bool sinksource)
    : size_(matrix->num_rows())
    , moves_(moves)
    , ssmove_()
    , queue_({{Depth(), matrix}})
    , map_({{matrix, Depth()}})
    , depth_()
    , sinksource_(sinksource) {}

std::shared_ptr<EquivQuiverMatrix>
MoveClassLoader::next() {
  typedef MMIMove::Applicable MMIApp;
  typedef std::vector<MMIApp> MMIAppVec;
  typedef SSMove::Applicable SSApp;
  typedef std::vector<SSApp> SSAppVec;
  auto begin = queue_.begin();
  MPtr result = (*begin).second;
  queue_.erase(begin);
  depth_ = map_[result];
  if (sinksource_) {
    SSAppVec ssapp = ssmove_.applicable_submatrices(result);
    for (auto app = ssapp.begin(); app != ssapp.end(); ++app) {
      MPtr ss_new = std::make_shared<M>(size_, size_);
      ssmove_.move(*app, *ss_new);
      if (map_.find(ss_new) == map_.end()) {
        Depth d = Depth(depth_.moves_, depth_.sinksource_ + 1);
        map_[ss_new] = d;
        queue_.insert(std::make_pair(d, ss_new));
      }
    }
  }
  for (auto i = moves_.begin(); i != moves_.end(); ++i) {
    MovePtr m = *i;
    MMIAppVec appl = m->applicable_submatrices(result);
    for (size_t j = 0; j < appl.size(); ++j) {
      MPtr new_matrix = std::make_shared<M>(size_, size_);
      m->move(appl[j], *new_matrix);
      if (map_.find(new_matrix) == map_.end()) {
        Depth d = Depth(depth_.moves_ + 1, depth_.sinksource_);
        map_[new_matrix] = d;
        queue_.insert(std::make_pair(d, new_matrix));
      }
    }
  }
  return result;
}

}  // namespace cluster
