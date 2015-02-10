/*
 * move_graph.cc
 *
 * Copyright 2015 John Lawson
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
#include "move_graph.h"

#include "equiv_quiver_matrix.h"

namespace cluster {
namespace {
typedef std::vector<MMIMove::Applicable> AVec;
}

template<class M>
MoveGraph<M>::MoveGraph(const M & mat, MoveVec moves)
		: _matrix(mat), _moves(std::move(moves)) {
	UMatrixPtr m(new M(mat));
	_queue.push_back(m);
	_map.emplace(m, Link(_queue.front()));
	_GraphLoader l(*this);
	while(!_queue.empty() && !l.end()) {
		++l;
	}
}
template<class M> typename
MoveGraph<M>::_GraphLoader & MoveGraph<M>::_GraphLoader::operator++(){
	MatrixPtr mat = _graph._queue.front();
	_graph._queue.pop_front();
	for (auto move_it = _graph._moves.begin();
			move_it != _graph._moves.end();
			++move_it) {
		AVec applicable = move_it->applicable_submatrices(
				std::make_shared<IntMatrix>(*mat));
		for(auto app_it = applicable.begin();
				app_it != applicable.end(); ++app_it) {
			UMatrixPtr new_matrix(new M(_size, _size));
			move_it->move(*app_it, *new_matrix);
			typename GraphMap::const_iterator seen = _graph._map.find(new_matrix);
			if (seen != _graph._map.end()) {
				seen_matrix(seen, mat);
				delete new_matrix;
			} else {
				unseen_matrix(new_matrix, mat);
			}
		}
	}
	return *this;
}
template<class M>
void MoveGraph<M>::_GraphLoader::seen_matrix(
		const typename GraphMap::const_iterator & new_mat,
		MatrixPtr old_mat) {
	_graph._map[new_mat->first].add_link(old_mat);
	_graph._map[old_mat].add_link(new_mat->first);
}
template<class M>
void MoveGraph<M>::_GraphLoader::unseen_matrix(const UMatrixPtr & new_mat,
		MatrixPtr old_mat) {
	_graph._map.emplace(std::pair<MatrixPtr, Link>(new_mat, Link(new_mat)));
	_graph._map[new_mat].add_link(old_mat);
	_graph._map[old_mat].add_link(new_mat);
	_graph._queue.push_back(new_mat);
}
}
namespace cluster {
template class MoveGraph<EquivQuiverMatrix>;
}

