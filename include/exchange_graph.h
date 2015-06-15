/*
 * quiver_graph.h
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
 * Computes an entire quiver graph and keeps all quivers in a map. This is
 * probably slower than the mutation_class equivalent, but those get rid of
 * unneeded quivers.
 */
#pragma once

#include <deque>
#include <memory>
#include <unordered_map>
#include <vector>

namespace cluster {

template<class Matrix>
class ExchangeGraph {
public:
	typedef Matrix * MatrixPtr;
private:
	class _GraphLoader;
	class _Link;
	/** Compares two matrices using their equals method.  */
	struct PtrEqual {
		bool operator()(MatrixPtr lhs, MatrixPtr rhs) const {
			return lhs->equals(*rhs);
		}
	};
	/** Gets hash using Matrix::hash method */
	struct PtrHash {
		size_t operator()(MatrixPtr ptr) const {
			return ptr->hash();
		}
	};
public:
	typedef _GraphLoader loader;
	typedef _Link Link;
	typedef std::pair<Matrix, Link> value_type;
	typedef std::unordered_map<MatrixPtr, Link, PtrHash, PtrEqual> GraphMap;

	/** Construct an empty graph which contains nothing. */
	ExchangeGraph() = default;
	/** Construct the exchange graph for the specified matrix. */
	ExchangeGraph(const Matrix & mat);
	/** Delete any matrix pointers controlled by this graph. */
	~ExchangeGraph() {
		for(auto it = _map.begin(); it != _map.end(); ++it) {
			delete it->first;
		}
	}
	const typename GraphMap::const_iterator
	begin() const {
		return _map.begin();
	}
	const typename GraphMap::const_iterator
	end() const {
		return _map.end();
	}
private:
	/** Initial matrix */
	const Matrix & _matrix;
	/** Map of matrices and their links. */
	GraphMap _map;
	/** Queue of matrices to mutate next. */
	std::deque<MatrixPtr> _queue;
	/** 
	 * GraphLoader actually computes the whole graph. The Graph ctor loops on
	 * the ::operator++ until ::end() returns true.
	 */
	class _GraphLoader {
	public:
		_GraphLoader() = default;

		_GraphLoader(ExchangeGraph & graph)
			: _graph(graph),
				_size(_graph._matrix.num_rows()) {}
		/** Load the next section of the graph. */
		loader & operator++();
		/** Return whether the whole graph been loaded. */
		bool end() const {
			return _end;
		}
	private:
		ExchangeGraph & _graph;
		bool _end = false;
		int _size;

		bool have_seen(MatrixPtr new_mat);
		bool mutate_at(MatrixPtr old_mat, int vertex);
		void seen_matrix(MatrixPtr new_mat, MatrixPtr old_mat, int vertex);
		void unseen_matrix(MatrixPtr new_mat, MatrixPtr old_mat, int vertex);
		/** Return true if the two Matrices are the same (not up to permutation) */
		bool is_exactly(MatrixPtr lhs, MatrixPtr rhs);
		/** Create a new Matrix to use in mutation. */
		MatrixPtr new_instance();
		/** Get the size of the matrices in the graph. */
		size_t size(MatrixPtr m) const {
			return m->size();
		}
	};
	/**
	 * Link stores the edges adjacent to its matrix.
	 */
	class _Link {
		typedef std::vector<MatrixPtr> LinkVec;
	public:
		_Link() :_matrix(nullptr) {};
		_Link(MatrixPtr matrix) : _matrix(matrix),
			_links(matrix->num_rows(), nullptr){}

		MatrixPtr _matrix;

		MatrixPtr & operator[](int i) { return _links[i]; }
		const typename LinkVec::const_iterator begin() const {
			return _links.begin();
		}
		const typename LinkVec::const_iterator end() const {
			return _links.end();
		}
	private:
		LinkVec _links;
	};
};
template<class M>
ExchangeGraph<M>::ExchangeGraph(const M & mat) : _matrix(mat) {
	MatrixPtr m(new M(mat));
	_queue.push_back(m);
	_map.emplace(m, Link(_queue.front()));
	_GraphLoader l(*this);
	while(!l.end()) {
		++l;
	}
}
template<class M> typename
ExchangeGraph<M>::_GraphLoader &
ExchangeGraph<M>::_GraphLoader::operator++(){
	MatrixPtr mat = _graph._queue.front();
	_graph._queue.pop_front();
	for (int i = 0; i < _size && !_end; ++i) {
		if (mutate_at(mat, i)) {
			MatrixPtr new_matrix = new_instance();
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
bool
ExchangeGraph<M>::_GraphLoader::have_seen(MatrixPtr new_mat) {
	return _graph._map.find(new_mat) != _graph._map.end();
}
template<class M>
bool
ExchangeGraph<M>::_GraphLoader::mutate_at(MatrixPtr old_mat, int vertex) {
	bool result = _graph._map.find(old_mat) != _graph._map.end() 
		&& _graph._map[old_mat][vertex] == nullptr;
	return result;
}
template<class M>
void
ExchangeGraph<M>::_GraphLoader::seen_matrix(MatrixPtr new_mat,
		MatrixPtr old_mat, int vertex) {
	auto ref = _graph._map.find(new_mat);
	if(ref != _graph._map.end() && is_exactly(*new_mat, *(ref->first))) {
		_graph._map[new_mat][vertex] = old_mat;
	}
	if(_graph._map[old_mat][vertex] == nullptr) {
		_graph._map[old_mat][vertex] = ref->first;
	}
}
template<class M>
void
ExchangeGraph<M>::_GraphLoader::unseen_matrix(MatrixPtr new_mat,
		MatrixPtr old_mat, int vertex) {
	_graph._map.emplace(new_mat, _Link(new_mat));
	_graph._map[new_mat][vertex] = old_mat;
	_graph._map[old_mat][vertex] = new_mat;
	_graph._queue.push_back(new_mat);
}
}

