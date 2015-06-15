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
ExchangeGraph<Seed>::_GraphLoader::new_instance() {
	return new Seed(_size);
}
template<>
bool
ExchangeGraph<Seed>::_GraphLoader::is_exactly(
		MatrixPtr lhs,
		MatrixPtr rhs) {
	return false;
}
template<>
LabelledSeed *
ExchangeGraph<LabelledSeed>::_GraphLoader::new_instance() {
	return new LabelledSeed(_size);
}
template<>
bool
ExchangeGraph<LabelledSeed>::_GraphLoader::is_exactly(
		MatrixPtr /*ignored*/,
		MatrixPtr /*ignored*/) {
	/* 
	 * This is only used to see whether two seeds which are considered "equal" are
	 * in fact exactly the same. As LabelledSeeds are always exactly the same if
	 * considered equal we can short cut this method.
	 */
	return true;
}
}

