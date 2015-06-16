/*
 * quiver_graph.cc
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
#include "exchange_graph.h"

#include "seed.h"

namespace cluster {

template<>
Seed *
ExchangeGraph::_GraphLoader::new_instance() {
	return new Seed(_size);
}
template<>
bool
ExchangeGraph::_GraphLoader::is_exactly(
		MatrixPtr lhs,
		MatrixPtr rhs) {
	return IntMatrix::are_equal(lhs->matrix(), rhs->matrix())
		&& (lhs->cluster() == rhs->cluster());
}
template<>
size_t
ExchangeGraph::size(const Matrix & m) const {
	return m.size();
}
template<>
LabelledSeed *
LabelledExchangeGraph::_GraphLoader::new_instance() {
	return new LabelledSeed(_size);
}
template<>
bool
LabelledExchangeGraph::_GraphLoader::is_exactly(
		MatrixPtr /*ignored*/,
		MatrixPtr /*ignored*/) {
	/* 
	 * This is only used to see whether two seeds which are considered "equal" are
	 * in fact exactly the same. As LabelledSeeds are always exactly the same if
	 * considered equal we can short cut this method.
	 */
	return true;
}
template<>
size_t
LabelledExchangeGraph::size(const Matrix & m) const {
	return m.size();
}
template<>
QuiverMatrix *
LabelledQuiverGraph::_GraphLoader::new_instance() {
	return new QuiverMatrix(_size, _size);
}
template<>
bool
LabelledQuiverGraph::_GraphLoader::is_exactly(
		MatrixPtr /*ignored*/,
		MatrixPtr /*ignored*/) {
	/* 
	 * This is only used to see whether two quivers which are considered "equal" are
	 * in fact exactly the same. As QuiverMatrices are always exactly the same if
	 * considered equal we can short cut this method.
	 */
	return true;
}
template<>
size_t
LabelledQuiverGraph::size(const Matrix & m) const {
	return m.num_rows();
}
template<>
bool
LabelledQuiverGraph::_GraphLoader::should_stop(MatrixPtr new_matrix) {
	return new_matrix->is_infinite();
}
template<>
EquivQuiverMatrix *
QuiverGraph::_GraphLoader::new_instance() {
	return new EquivQuiverMatrix(_size, _size);
}
template<>
bool
QuiverGraph::_GraphLoader::is_exactly(
		MatrixPtr lhs,
		MatrixPtr rhs) {
	return IntMatrix::are_equal(*lhs, *rhs);
}
template<>
size_t
QuiverGraph::size(const Matrix & m) const {
	return m.num_rows();
}
template<>
bool
QuiverGraph::_GraphLoader::should_stop(MatrixPtr new_matrix) {
	return new_matrix->is_infinite();
}
}

