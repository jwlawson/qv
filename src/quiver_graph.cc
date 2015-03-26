/*
 * quiver_graph.cc
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
#include "quiver_graph.h"

#include "equiv_quiver_matrix.h"

namespace cluster {

template<class M>
QuiverGraph<M>::QuiverGraph(const M & mat) : _matrix(mat) {
	UMatrixPtr m(new M(mat));
	_queue.push_back(m);
	_map.emplace(m, Link(_queue.front()));
	_GraphLoader l(*this);
	while(!l.end()) {
		++l;
	}
}
template<class M> typename
QuiverGraph<M>::_GraphLoader & QuiverGraph<M>::_GraphLoader::operator++(){
	MatrixPtr mat = _graph._queue.front();
	_graph._queue.pop_front();
	for (int i = 0; i < _size && !_end; ++i) {
		if (mutate_at(mat, i)) {
			UMatrixPtr new_matrix(new M(_size, _size));
			mat->mutate(i, *new_matrix);
			if (have_seen(new_matrix)) {
				seen_matrix(new_matrix, mat, i);
				delete new_matrix;
			} else {
				if (new_matrix->is_infinite()) {
					_end = true;
				}
				unseen_matrix(new_matrix, mat, i);
			}
		}
	}
	if(_graph._queue.empty()){
		_end = true;
	}
	return *this;
}
template<class M>
bool QuiverGraph<M>::_GraphLoader::have_seen(const UMatrixPtr & new_mat) {
	return _graph._map.find(new_mat) != _graph._map.end();
}
template<class M>
bool QuiverGraph<M>::_GraphLoader::mutate_at(const MatrixPtr & old_mat, int vertex) {
	bool result = _graph._map.find(old_mat) != _graph._map.end() 
		&& _graph._map[old_mat][vertex] == nullptr;
	return result;
}
template<class M>
void QuiverGraph<M>::_GraphLoader::seen_matrix(const UMatrixPtr & new_mat,
		MatrixPtr old_mat, int vertex) {
	auto ref = _graph._map.find(new_mat);
	if(ref != _graph._map.end() && IntMatrix::are_equal(*new_mat, *(ref->first))) {
		_graph._map[new_mat][vertex] = old_mat;
	}
	if(_graph._map[old_mat][vertex] == nullptr) {
		_graph._map[old_mat][vertex] = ref->first;
	}
}
template<class M>
void QuiverGraph<M>::_GraphLoader::unseen_matrix(const UMatrixPtr & new_mat,
		MatrixPtr old_mat, int vertex) {
	_graph._map.emplace(std::pair<MatrixPtr, Link>(new_mat, _Link(new_mat)));
	_graph._map[new_mat][vertex] = old_mat;
	_graph._map[old_mat][vertex] = new_mat;
	_graph._queue.push_back(new_mat);
}
}

namespace cluster {
template class QuiverGraph<EquivQuiverMatrix>;
}

