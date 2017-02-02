/*
 * move_graph.cc
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
#include "ss_move.h"

namespace cluster {
namespace {
typedef std::vector<MMIMove::Applicable> AVec;
}

template<class M>
MoveGraph<M>::MoveGraph(const M & mat, MoveVec moves)
		: _matrix(mat), _moves(std::move(moves)) {
	CMatrixPtr m(new M(mat));
	add_ss_equiv(m);
	_map.emplace(m, Link(_queue.front()));
	_GraphLoader l(*this);
	while(!_queue.empty() && !l.end()) {
		++l;
	}
}
template<class M> typename
MoveGraph<M>::_GraphLoader & MoveGraph<M>::_GraphLoader::operator++(){
	CMatrixPtr mat = _graph._queue.front();
	_graph._queue.pop_front();
	CMatrixPtr in_map = _graph.ss_move_equiv(mat);
	for (auto move_it = _graph._moves.begin();
			move_it != _graph._moves.end();
			++move_it) {
		AVec applicable = move_it->applicable_submatrices(
				std::make_shared<IntMatrix>(*mat));
		for(auto app_it = applicable.begin();
				app_it != applicable.end(); ++app_it) {
			MatrixPtr new_matrix(new M(_size, _size));
			move_it->move(*app_it, *new_matrix);
			CMatrixPtr seen_map = _graph.ss_move_equiv(new_matrix);
			if (seen_map) {
				seen_matrix(seen_map, in_map);
				delete new_matrix;
			} else {
				unseen_matrix(new_matrix, in_map);
			}
		}
	}
	if(in_map != mat) {
		delete mat;
	}
	return *this;
}
template<class M>
void MoveGraph<M>::_GraphLoader::seen_matrix(
		CMatrixPtr new_mat,
		CMatrixPtr old_mat) {
	_graph._map[new_mat].add_link(old_mat);
	_graph._map[old_mat].add_link(new_mat);
}
template<class M>
void MoveGraph<M>::_GraphLoader::unseen_matrix(CMatrixPtr const& new_mat,
		CMatrixPtr old_mat) {
	_graph._map.emplace(std::pair<CMatrixPtr, Link>(new_mat, Link(new_mat)));
	_graph._map[new_mat].add_link(old_mat);
	_graph._map[old_mat].add_link(new_mat);
	_graph.add_ss_equiv(new_mat);
}
template<class M>
void MoveGraph<M>::add_ss_equiv(CMatrixPtr new_mat) {
	typedef std::unordered_set<CMatrixPtr, PtrHash, PtrEqual> Set;
	Set to_add;
	std::deque<CMatrixPtr> queue;
	queue.push_back(new_mat);
	to_add.insert(new_mat);
	SSMove move;
	while(!queue.empty()) {
		CMatrixPtr mat = queue.front();
		queue.pop_front();
		auto applicable = move.applicable_submatrices(*mat);
		for(auto it = applicable.begin();
				it != applicable.end();
				++it) {
			MatrixPtr pt = new M(mat->num_rows(),mat->num_cols());
			move.move(*it, *pt);
			auto ins = to_add.insert(pt);
			if(ins.second) {
				_queue.push_back(pt);
				queue.push_back(pt);
			} else {
				delete pt;
			}
		}
	}
}
template<class M>
typename MoveGraph<M>::CMatrixPtr MoveGraph<M>::ss_move_equiv(CMatrixPtr chk_mat) {
	auto ref = _map.find(chk_mat);
	if(ref != _map.end()) {
		return ref->first;
	}
	typedef std::unordered_set<CMatrixPtr, PtrHash, PtrEqual> Set;
	Set found;
	std::deque<CMatrixPtr> queue;
	queue.push_back(chk_mat);
	found.insert(chk_mat);
	SSMove move;
	CMatrixPtr result = nullptr;
	while(!queue.empty()) {
		CMatrixPtr mat = queue.front();
		queue.pop_front();
		auto applicable = move.applicable_submatrices(*mat);
		for(auto it = applicable.begin();
				!result && it != applicable.end();
				++it) {
			MatrixPtr pt = new M(mat->num_rows(),mat->num_cols());
			move.move(*it, *pt);
			auto ins = found.insert(pt);
			if(ins.second) {
				auto in_map = _map.find(pt);
				if(in_map != _map.end()) {
					result = in_map->first;
				}
				queue.push_back(pt);
			} else {
				delete pt;
			}
		}
	}
	for(auto i = found.begin(); i != found.end(); ++i) {
		if(*i != chk_mat) {
			delete *i;
		}
	}
	return result;
}
}
namespace cluster {
template class MoveGraph<EquivQuiverMatrix>;
}

