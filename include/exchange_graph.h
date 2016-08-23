/*
 * exchange_graph.h
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

#include <algorithm>
#include <deque>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "seed.h"

namespace cluster {

template<class Matrix, class ContinueMatrix>
class __ExchangeGraph {
public:
	typedef Matrix * MatrixPtr;
private:
	class _GraphLoader;
	class _Link;
	/** Compares two matrices using their equals method.  */
	struct PtrEqual {
		bool operator()(MatrixPtr lhs, MatrixPtr rhs) const;
	};
	/** Gets hash using Matrix::hash method */
	struct PtrHash {
		size_t operator()(MatrixPtr ptr) const;
	};
public:
	typedef _GraphLoader loader;
	typedef _Link Link;
	typedef std::pair<Matrix, Link> value_type;
	typedef std::unordered_map<MatrixPtr, Link, PtrHash, PtrEqual> GraphMap;

	/** Construct an empty graph which contains nothing. */
	__ExchangeGraph() = default;
	/** Construct the exchange graph for the specified matrix. */
	__ExchangeGraph(const Matrix & mat, size_t max_num = SIZE_MAX);
	/** Delete any matrix pointers controlled by this graph. */
	~__ExchangeGraph();
	const typename GraphMap::const_iterator begin() const;
	const typename GraphMap::const_iterator end() const;
private:
	/** Initial matrix */
	const Matrix & _matrix;
	/** Size of matrix. */
	const size_t _matrix_size;
	/** Maximum number of entries in the graph. */
	const size_t _max_num;
	/** Map of matrices and their links. */
	GraphMap _map;
	/** Queue of matrices to mutate next. */
	std::deque<MatrixPtr> _queue;
	/** 
	 * Get the size of the matrices in the graph.
	 * This is one of the methods which might need to be specialised, depending
	 * on the Matrix used.
	 */
	template<class M = Matrix>
	typename std::enable_if<std::is_same<M, Seed>::value
		|| std::is_same<M, LabelledSeed>::value, size_t>::type
	size(const M& m) const;
	template<class M = Matrix>
	typename std::enable_if<std::is_base_of<QuiverMatrix, M>::value, size_t>::type
	size(const M& m) const;
	/** 
	 * GraphLoader actually computes the whole graph. The Graph ctor loops on
	 * the ::operator++ until ::end() returns true.
	 */
	class _GraphLoader {
	public:
		_GraphLoader() = default;

		_GraphLoader(__ExchangeGraph & graph);
		/** Load the next section of the graph. */
		loader & operator++();
		/** Return whether the whole graph been loaded. */
		bool end() const;
	private:
		__ExchangeGraph & _graph;
		bool _end = false;
		size_t _size;
		ContinueMatrix _should_continue;

		bool have_seen(MatrixPtr new_mat);
		bool mutate_at(MatrixPtr old_mat, int vertex);
		void seen_matrix(MatrixPtr new_mat, MatrixPtr old_mat, int vertex);
		void unseen_matrix(MatrixPtr new_mat, MatrixPtr old_mat, int vertex);
		/*
		 * The following methods are those that might need to be sepcialised
		 * depending on the classes used to construct the exchange graph.
		 * e.g. Seeds may need to be handled differently than Quivers.
		 */
		/** Return true if the two Matrices are the same (not up to permutation) */
		template<class M = Matrix>
		typename std::enable_if<std::is_same<M, Seed>::value, bool>::type
		is_exactly(MatrixPtr lhs, MatrixPtr rhs);
		template<class M = Matrix>
		typename std::enable_if<std::is_same<M, LabelledSeed>::value ||
				std::is_same<QuiverMatrix, M>::value, bool>::type
		is_exactly(MatrixPtr lhs, MatrixPtr rhs);
		template<class M = Matrix>
		typename std::enable_if<std::is_same<EquivQuiverMatrix, M>::value, bool>::type
		is_exactly(MatrixPtr lhs, MatrixPtr rhs);
		/** Create a new Matrix to use in mutation. */
		template<class M = Matrix>
		typename std::enable_if<std::is_same<M, Seed>::value || std::is_same<M, LabelledSeed>::value, M*>::type
		new_instance();
		template<class M = Matrix>
		typename std::enable_if<std::is_base_of<QuiverMatrix, M>::value, M*>::type
		new_instance();
		/** Check whether to stop computing further Matrices in the graph. */
		template<class M = Matrix>
		typename std::enable_if<!std::is_base_of<QuiverMatrix, M>::value, bool>::type
		should_stop(MatrixPtr new_matrix);
		template<class M = Matrix>
		typename std::enable_if<std::is_base_of<QuiverMatrix, M>::value, bool>::type
		should_stop(MatrixPtr new_matrix);
	};
	/**
	 * Link stores the edges adjacent to its matrix.
	 */
	class _Link {
		typedef std::vector<MatrixPtr> LinkVec;
	public:
		/** Construct link for null matrix. */
		_Link();
		/** Construct link for given matrix. */
		_Link(MatrixPtr matrix, int size);
		/** Get link at i */
		MatrixPtr & operator[](int i);
		const typename LinkVec::const_iterator begin() const;
		const typename LinkVec::const_iterator end() const;
		/** Inital matrix */
		MatrixPtr _matrix;
	private:
		LinkVec _links;
	};
};
/* ExchangeGraph */
template<class M, class C>
inline
bool
__ExchangeGraph<M,C>::PtrEqual::operator()(MatrixPtr lhs, MatrixPtr rhs) const {
	return lhs->equals(*rhs);
}
template<class M, class C>
inline
size_t
__ExchangeGraph<M,C>::PtrHash::operator()(MatrixPtr ptr) const {
return ptr->hash();
}
template<class M, class C>
inline
__ExchangeGraph<M,C>::__ExchangeGraph(const M & mat, size_t max_num)
: _matrix(mat), _matrix_size(size(mat)), _max_num(max_num) {
	MatrixPtr m(new M(mat));
	_queue.push_back(m);
	_map.emplace(m, Link(_queue.front(), _matrix_size));
	_GraphLoader l(*this);
	while(!l.end()) {
		++l;
	}
}
template<class M, class C>
inline
__ExchangeGraph<M,C>::~__ExchangeGraph() {
	for(auto it = _map.begin(); it != _map.end(); ++it) {
		delete it->first;
	}
}
template<class M, class C>
inline
const typename __ExchangeGraph<M,C>::GraphMap::const_iterator
__ExchangeGraph<M,C>::begin() const {
	return _map.begin();
}
template<class M, class C>
inline
const typename __ExchangeGraph<M,C>::GraphMap::const_iterator
__ExchangeGraph<M,C>::end() const {
	return _map.end();
}
/* _GraphLoader */
template<class M, class C>
inline
__ExchangeGraph<M,C>::_GraphLoader::_GraphLoader(__ExchangeGraph & graph)
: _graph(graph), _size(graph._matrix_size) {}

template<class M, class C>
inline
typename __ExchangeGraph<M,C>::_GraphLoader &
__ExchangeGraph<M,C>::_GraphLoader::operator++(){
	MatrixPtr mat = _graph._queue.front();
	_graph._queue.pop_front();
	for (std::size_t i = 0; i < _size && !_end; ++i) {
		if (mutate_at(mat, i)) {
			MatrixPtr new_matrix = new_instance();
			mat->mutate(i, *new_matrix);
			if (have_seen(new_matrix)) {
				seen_matrix(new_matrix, mat, i);
				delete new_matrix;
			} else {
				if (should_stop(new_matrix)) {
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
template<class M, class C>
inline
bool
__ExchangeGraph<M,C>::_GraphLoader::end() const {
	return _end;
}
template<class M, class C>
inline
bool
__ExchangeGraph<M,C>::_GraphLoader::have_seen(MatrixPtr new_mat) {
	return _graph._map.find(new_mat) != _graph._map.end();
}
template<class M, class C>
inline
bool
__ExchangeGraph<M,C>::_GraphLoader::mutate_at(MatrixPtr old_mat, int vertex) {
	bool result = _should_continue(old_mat, vertex)
		&& _graph._map.find(old_mat) != _graph._map.end()
		&& _graph._map[old_mat][vertex] == nullptr;
	return result;
}
template<class M, class C>
inline
void
__ExchangeGraph<M,C>::_GraphLoader::seen_matrix(MatrixPtr new_mat,
		MatrixPtr old_mat, int vertex) {
	auto ref = _graph._map.find(new_mat);
	if(ref != _graph._map.end() && is_exactly(new_mat, ref->first)) {
		_graph._map[new_mat][vertex] = old_mat;
	}
	if(_graph._map[old_mat][vertex] == nullptr) {
		_graph._map[old_mat][vertex] = ref->first;
	}
}
template<class M, class C>
inline
void
__ExchangeGraph<M,C>::_GraphLoader::unseen_matrix(MatrixPtr new_mat,
		MatrixPtr old_mat, int vertex) {
	_graph._map.emplace(new_mat, _Link(new_mat, _size));
	_graph._map[new_mat][vertex] = old_mat;
	_graph._map[old_mat][vertex] = new_mat;
	_graph._queue.push_back(new_mat);
}
/* _Link */
template<class M, class C>
inline
__ExchangeGraph<M,C>::_Link::_Link() :_matrix(nullptr) {}

template<class M, class C>
inline
__ExchangeGraph<M,C>::_Link::_Link(MatrixPtr matrix, int size)
: _matrix(matrix), _links(size, nullptr){}

template<class M, class C>
inline
typename __ExchangeGraph<M,C>::MatrixPtr &
__ExchangeGraph<M,C>::_Link::operator[](int i) {
	return _links[i];
}
template<class M, class C>
inline
const typename __ExchangeGraph<M,C>::_Link::LinkVec::const_iterator
__ExchangeGraph<M,C>::_Link::begin() const {
	return _links.begin();
}
template<class M, class C>
inline
const typename __ExchangeGraph<M,C>::_Link::LinkVec::const_iterator
__ExchangeGraph<M,C>::_Link::end() const {
	return _links.end();
}

template<class Matrix, class Check> template<class M>
inline
typename std::enable_if<std::is_same<M, Seed>::value
		|| std::is_same<M, LabelledSeed>::value, M*>::type
__ExchangeGraph<Matrix, Check>::_GraphLoader::new_instance() {
	return new M(_size);
}
template<class Matrix, class Check> template<class M>
inline
typename std::enable_if<std::is_base_of<QuiverMatrix, M>::value, M*>::type
__ExchangeGraph<Matrix, Check>::_GraphLoader::new_instance() {
	return new M(_size, _size);
}
template<class Matrix, class Check> template<class M>
inline
typename std::enable_if<!std::is_base_of<QuiverMatrix, M>::value, bool>::type
__ExchangeGraph<Matrix, Check>::_GraphLoader::should_stop(MatrixPtr /*ignored*/) {
	size_t num = _graph._map.size();
	return num > _graph._max_num;
}
template<class Matrix, class Check> template<class M>
inline
typename std::enable_if<std::is_base_of<QuiverMatrix, M>::value, bool>::type
__ExchangeGraph<Matrix, Check>::_GraphLoader::should_stop(MatrixPtr new_matrix) {
	return new_matrix->is_infinite();
}
template<class Matrix, class Check> template<class M>
inline
typename std::enable_if<std::is_same<M, Seed>::value
		|| std::is_same<M, LabelledSeed>::value, size_t>::type
__ExchangeGraph<Matrix, Check>::size(const M& m) const {
	return m.size();
}
template<class Matrix, class Check> template<class M>
inline
typename std::enable_if<std::is_base_of<QuiverMatrix, M>::value, size_t>::type
__ExchangeGraph<Matrix, Check>::size(const M& m) const {
	return m.num_rows();
}
template<class Matrix, class Check> template<class M>
inline
typename std::enable_if<std::is_same<M, Seed>::value, bool>::type
__ExchangeGraph<Matrix, Check>::_GraphLoader::is_exactly(
		MatrixPtr lhs,
		MatrixPtr rhs) {
	return IntMatrix::are_equal(lhs->matrix(), rhs->matrix())
		&& (lhs->cluster() == rhs->cluster());
}
template<class Matrix, class Check> template<class M>
inline
typename std::enable_if<std::is_same<M, LabelledSeed>::value ||
		std::is_same<QuiverMatrix, M>::value, bool>::type
__ExchangeGraph<Matrix, Check>::_GraphLoader::is_exactly(
		MatrixPtr /*ignored*/,
		MatrixPtr /*ignored*/) {
	/* 
	 * This is only used to see whether two quivers which are considered "equal" are
	 * in fact exactly the same. As QuiverMatrices are always exactly the same if
	 * considered equal we can short cut this method.
	 */
	return true;
}
template<class Matrix, class Check> template<class M>
inline
typename std::enable_if<std::is_same<EquivQuiverMatrix, M>::value, bool>::type
__ExchangeGraph<Matrix, Check>::_GraphLoader::is_exactly(
		MatrixPtr lhs,
		MatrixPtr rhs) {
	return IntMatrix::are_equal(*lhs, *rhs);
}



namespace _EGContinueChecks {
struct AlwaysContinue {
	bool operator()(void * /* ignored */, int /* ignored */) const {
		return true;
	}
};
struct InfiniteTypeSink {
	bool operator()(QuiverMatrix const * const mptr, int /*vertex*/) const {
		for(int i = 0; i < mptr->num_rows(); ++i) {
			_vertex_cache[i].clear();
		}
		const int * row = mptr->data();
		for(int c = 0, r = 0; r < mptr->num_rows(); ++r) {
			for(c = 0; c < mptr->num_cols(); ++c) {
				if(*(row++) > 1) {
					_vertex_cache[r].push_back(c);
				}
			}
		}
		for(auto pair : _vertex_cache) {
			for(int next_vert : pair.second) {
				for(int third_vert : _vertex_cache[next_vert]) {
					if(std::find(_vertex_cache[third_vert].begin(),
								_vertex_cache[third_vert].end(), pair.first) !=
							_vertex_cache[third_vert].end()) {
						return false;
					}
				}
			}
		}
		/*
		const int * row_start = mptr->data() + (vertex*mptr->num_cols());
		for(int i = 0; i < mptr->num_cols(); ++i) {
			if(row_start[i] < -1) return false;
		}
		*/
		return true;
	}
	bool operator()(Seed const * const sptr, int vertex) const {
		return operator()(&(sptr->matrix()), vertex);
	}
	bool operator()(LabelledSeed const * const sptr, int vertex) const {
		return operator()(&(sptr->matrix()), vertex);
	}
	mutable std::map<int, std::vector<int>> _vertex_cache;
};
}
template <class M>
using  _EGAlways = __ExchangeGraph<M, _EGContinueChecks::AlwaysContinue>;
template <class M>
using  _EGGreen = __ExchangeGraph<M, _EGContinueChecks::InfiniteTypeSink>;
typedef _EGAlways<Seed> ExchangeGraph;
typedef _EGAlways<LabelledSeed> LabelledExchangeGraph;
typedef _EGAlways<QuiverMatrix> LabelledQuiverGraph;
typedef _EGAlways<EquivQuiverMatrix> QuiverGraph;
typedef _EGGreen<Seed> GreenExchangeGraph;
typedef _EGGreen<LabelledSeed> GreenLabelledExchangeGraph;
typedef _EGGreen<QuiverMatrix> GreenLabelledQuiverGraph;
typedef _EGGreen<EquivQuiverMatrix> GreenQuiverGraph;
}

