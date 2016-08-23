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

#ifdef QV_USE_GINAC
#include "ginac/ginac.h"
#endif

#include <cmath>

namespace cluster {
template<class Mat>
class __Seed {
public:
#ifdef QV_USE_GINAC
	typedef std::vector<GiNaC::ex> Cluster;
#else
	typedef std::vector<int> Cluster;
#endif
	typedef Mat Matrix;

	/** Construct 'empty' instance with specified size. */
	__Seed(int size);
	/** Construct seed with the specified matrix and cluster. */
	__Seed(const IntMatrix & mat, const Cluster & cluster);
	/** Construct seed with the specified matrix and cluster. */
	__Seed(IntMatrix && mat, Cluster && cluster);
	/**
	 * Always call reset after changing the seed in any way. This ensures that the
	 * hascode produced actually corresponds to the data in the seed.
	 */
	void reset();
	/**
	 * Mutate this seed at the specified vertex. The resulting seed is
	 * stored in the provided seed.
	 * @param k Vertex to mutate at
	 * @param result Matrix to store result in
	 */
	void mutate(const int k, __Seed<Matrix> & result) const;
	/** Return the hash code for this seed. */
	size_t hash() const;
	/** Check whether the given seed is equivalent to this. */
	bool equals(const __Seed<Matrix> & seed) const;
	/** Return the number of variables in the seed. */
	size_t size() const;
	/** Access a reference to the Seed matrix. */
	const Matrix & matrix() const;
	/** Access a reference to the Seed cluster. */
	const Cluster & cluster() const;
	/** Output Seed to output stream. */
	template<class M>
	friend std::ostream &
	operator<<(std::ostream & os, const __Seed<M> & seed);
private:
	/** Size of the seed */
	size_t size_;
	/** Matrix/Quiver of the seed. */
	Matrix matrix_;
	/** Cluster of the seed. */
	Cluster cluster_;
	/** Cached Hashcode. */
	size_t hashcode_;
	/** Recompute the cached hashcode if the instance is changed. */
	size_t compute_hash() const;
	/** Perform mutation at the vertex k, on the cluster. */
	void mutate_cluster(const int k, Cluster & result) const;
};
template<class Matrix>
__Seed<Matrix>::__Seed(int size)
: size_(size), matrix_(size, size), cluster_(size) {
	reset();
}
template<class Matrix>
__Seed<Matrix>::__Seed(const IntMatrix & mat, const Cluster & cluster)
: size_(mat.num_rows()), matrix_(mat), cluster_(cluster) {
	reset();
}
template<class Matrix>
__Seed<Matrix>::__Seed(IntMatrix && mat, Cluster && cluster)
: size_(mat.num_rows()), matrix_(std::move(mat)), cluster_(cluster) {
	reset();
}
template<class Matrix>
void
__Seed<Matrix>::reset() {
	matrix_.reset();
	hashcode_ = compute_hash();
}
template<class Matrix>
void
__Seed<Matrix>::mutate(const int k, __Seed<Matrix> & result) const {
	mutate_cluster(k, result.cluster_);
	matrix_.mutate(k, result.matrix_);
	result.reset();
}
template<class Matrix>
size_t
__Seed<Matrix>::hash() const {
	return hashcode_;
}
template<class Matrix>
size_t
__Seed<Matrix>::size() const {
	return size_;
}
template<class Matrix>
const Matrix &
__Seed<Matrix>::matrix() const {
	return matrix_;
}
template<class Matrix>
const typename __Seed<Matrix>::Cluster &
__Seed<Matrix>::cluster() const {
	return cluster_;
}
/**
 * Need to use entries in k-th column to compute mutation.
 *
 * For skew-symmetric matrices it doesn't matter whether we use the row or
 * column, but it does for skew-symmetrizable matrices.
 */
template<class Matrix>
void
__Seed<Matrix>::mutate_cluster(const int k, Cluster & result) const {
	using std::pow;
#ifdef QV_USE_GINAC
	GiNaC::ex pos = 1;
	GiNaC::ex neg = 1;
#else
	int pos = 1;
	int neg = 1;
#endif
	/* Get pointer to mutating column in matrix */
	const int* data = matrix_.data() + k;

	for(size_t j = 0; j < size_; j++){
		if(*data > 0){
			pos *= pow(cluster_[j], *data);
		} else if (*data < 0){
			neg *= pow(cluster_[j], -1 * (*data));
		}
		data += size_;
	}
	result.resize(size_);
	for(size_t i = 0; i < size_; ++i) {
		result[i] = cluster_[i];
	}
#ifdef QV_USE_GINAC
	result[k] = ((pos+neg)/cluster_[k]).normal();
#else
	result[k] = ((pos+neg)/cluster_[k]);
#endif
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

template<>
inline
size_t
Seed::compute_hash() const {
	size_t result = matrix_.hash();
	size_t exp = 31;
	for(size_t i = 0; i < size_; ++i) {
#ifdef QV_USE_GINAC
		result += exp * (cluster_[i].gethash());
#else
		result += exp * (cluster_[i]);
#endif
	}
	return result;
}
template<>
inline
size_t
LabelledSeed::compute_hash() const {
	size_t result = matrix_.hash();
	size_t exp = 1;
	size_t pow = 31;
	for(size_t i = 0; i < size_; ++i) {
		exp *= pow;
#ifdef QV_USE_GINAC
		result += exp * (cluster_[i].gethash());
#else
		result += exp * (cluster_[i]);
#endif
	}
	return result;
}
template<>
inline
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
inline
bool
LabelledSeed::equals(const LabelledSeed & seed) const {
	return hash() == seed.hash() &&
		matrix_.equals(seed.matrix_) &&
		cluster_ == seed.cluster_;
}
}

