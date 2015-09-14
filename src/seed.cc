/*
 * seed.cc
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
#include "seed.h"

namespace cluster {

/*
 * This could probably be written in a more efficient way using the knowledge
 * that the quiver is determined by the cluster. Hence you only need to check
 * whether the clusters are permutations of each other. If you then wanted to be
 * extra safe, check that the matrices are equal under that permutation.
 */
template<>
bool
Seed::equals(const Seed & seed) const {
	bool result = hash() == seed.hash() && matrix_.equals(seed.matrix_);
	if(result) {
		EquivQuiverMatrix::PermVecPtr pv = matrix_.all_permutations(seed.matrix_);
		result = false;
		for(auto it = pv->begin(); !result && it != pv->end(); ++it) {
			EquivQuiverMatrix::Permutation & perm = *it;
			bool p_res = true;
			for(size_t i = 0; p_res && i < perm.size(); ++i) {
				p_res = cluster_[i] == seed.cluster_[perm[i]];
			}
			result = p_res;
		}
	}
	return result;
}
template<>
size_t
Seed::compute_hash() const {
	size_t result = matrix_.hash();
	size_t exp = 31;
	for(size_t i = 0; i < size_; ++i) {
		result += exp * (cluster_[i].gethash());
	}
	return result;
}
template<>
bool
LabelledSeed::equals(const LabelledSeed & seed) const {
	return hash() == seed.hash() &&
		matrix_.equals(seed.matrix_) && 
		cluster_ == seed.cluster_;
}
template<>
size_t
LabelledSeed::compute_hash() const {
	size_t result = matrix_.hash();
	size_t exp = 1;
	size_t pow = 31;
	for(size_t i = 0; i < size_; ++i) {
		exp *= pow;
		result += exp * (cluster_[i].gethash());
	}
	return result;
}
}

