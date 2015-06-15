/*
 * seed.h
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
 * A seed consists of a quiver and tuple of rational functions. The mutations
 * follow the x-variable mutations and standard quiver mutation. Two seed are
 * considered equal if they are the same up to a permutation of the cluster and
 * quiver.
 */
#pragma once

#include "equiv_quiver_matrix.h"

#include "ginac/ginac.h"

namespace cluster {
template<class Matrix>
class __Seed {
public:
	typedef std::vector<GiNaC::ex> Cluster;

	__Seed(int size)
		: size_(size),
		  matrix_(size, size),
		  cluster_(size) {
		reset();
	}
	__Seed(const IntMatrix & mat, const std::vector<GiNaC::ex> & cluster)
		: size_(mat.num_rows()),
		  matrix_(mat),
			cluster_(cluster) {
		reset();
	}
	__Seed(IntMatrix && mat, Cluster && cluster)
		: size_(mat.num_rows()),
		  matrix_(mat),
			cluster_(cluster) {
		reset();
	}
	/**
	 * Always call reset after changing the seed in any way. This ensures that the
	 * hascode produced actually corresponds to the data in the seed.
	 */
	void
	reset() {
		matrix_.reset();
		hashcode_ = compute_hash();
	}
	/**
	 * Mutate this matrix at the specified vertex. The resulting matrix is
	 * stored in the provided matrix.
	 * @param k Vertex to mutate at
	 * @param result Matrix to store result in
	 */
	void
	mutate(const int k, __Seed<Matrix> & result) const {
		mutate_cluster(k, result.cluster_);
		matrix_.mutate(k, result.matrix_);
		result.reset();
	}
	/** Return the hash code for this seed. */
	size_t
	hash() const {
		return hashcode_;
	}
	/** Check whether the given seed is equivalent to this. */
	bool
	equals(const __Seed<Matrix> & seed) const;
	/** Return the number of variables in the seed. */
	size_t
	size() const {
		return size_;
	}
	template<class M>
	friend std::ostream &
	operator<<(std::ostream & os, const __Seed<M> & seed);
private:
	size_t size_;
	Matrix matrix_;
	Cluster cluster_;
	size_t hashcode_;

	size_t
	compute_hash() const;
	void
	mutate_cluster(const int k, Cluster & result) const;
};
template<class Matrix>
void
__Seed<Matrix>::mutate_cluster(const int k, Cluster & result) const {
	GiNaC::ex pos = 1;
	GiNaC::ex neg = 1;
	/* Get pointer to mutating row in matrix */
	const int* data = matrix_.data() + (k * size_);

	for(size_t j = 0; j < size_; j++){
		if(*data > 0){
			pos *= pow(cluster_[j], *data);
		} else if (*data < 0){
			neg *= pow(cluster_[j], -1 * (*data));
		}
		++data;
	}
	result.resize(size_);
	for(size_t i = 0; i < size_; ++i) {
		result[i] = cluster_[i];
	}
	result[k] = (pos+neg)/cluster_[k];
}
template<class Matrix>
std::ostream &
operator<<(std::ostream & os, const __Seed<Matrix> & seed) {
	os << "{ ";
	for(size_t i = 0; i < seed.size_; ++i) {
		os << seed.cluster_[i] << " ";
	}
	os << "} " << seed.matrix_;
	return os;
}

typedef __Seed<EquivQuiverMatrix> Seed;
typedef __Seed<QuiverMatrix> LabelledSeed;
}

