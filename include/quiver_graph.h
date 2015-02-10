/*
 * quiver_graph.h
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
class QuiverGraph {
	public:
		typedef Matrix* MatrixPtr;
		typedef MatrixPtr UMatrixPtr;
	private:
		class _GraphLoader;
		class _Link;
		struct PtrEqual {
			bool operator()( const UMatrixPtr & lhs, const UMatrixPtr & rhs) const {
				return lhs->equals(*rhs);
			}
		};
		struct PtrHash {
			size_t operator()(const UMatrixPtr & ptr) const {
				return ptr->hash();
			}
		};

	public:
		typedef _GraphLoader loader;
		typedef _Link Link;
		typedef std::pair<Matrix, Link> value_type;
		typedef std::unordered_map<UMatrixPtr, Link, PtrHash, PtrEqual> GraphMap;

		QuiverGraph() = default;
		QuiverGraph(const Matrix & mat);
		~QuiverGraph() {
			for(auto it = _map.begin(); it != _map.end(); ++it) {
				delete it->first;
			}
		}
		const typename GraphMap::const_iterator begin() const { return _map.begin(); }
		const typename GraphMap::const_iterator end() const { return _map.end(); }

	private:
		const Matrix & _matrix;
		GraphMap _map;
		std::deque<MatrixPtr> _queue;

		class _GraphLoader {
			public:
				_GraphLoader() = default;

				_GraphLoader(QuiverGraph & graph)
					: _graph(graph), _size(_graph._matrix.num_rows()) {}

				loader & operator++();

				bool end() const {return _end;}

			private:
				QuiverGraph & _graph;
				bool _end = false;
				int _size;

				bool have_seen(const UMatrixPtr & new_mat);
				bool mutate_at(const MatrixPtr & old_mat, int vertex);
				void seen_matrix(const UMatrixPtr & new_mat, MatrixPtr old_mat,
						int vertex);
				void unseen_matrix(const UMatrixPtr & new_mat, MatrixPtr old_mat,
						int vertex);
		};

		class _Link {
			public:
				_Link() :_matrix(nullptr) {};
				_Link(MatrixPtr matrix) : _matrix(matrix),
					_links(matrix->num_rows(), nullptr){}

				const MatrixPtr _matrix;

				MatrixPtr & operator[](int i) { return _links[i]; }
				const MatrixPtr & operator[](int i) const { return _links[i]; }
				const typename std::vector<MatrixPtr>::const_iterator begin() const {
					return _links.begin();
				}
				const typename std::vector<MatrixPtr>::const_iterator end() const {
					return _links.end();
				}

			private:
				std::vector<MatrixPtr> _links;
		};

};
}

