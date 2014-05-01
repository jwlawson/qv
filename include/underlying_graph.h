/*
 * underlying_graph.h
 *
 * Contains the UnderlyingGraph class.
 *
 * Underlying graph provides an easy way to construct the matrix of the
 * underlying graph from a quiver.
 */
#pragma once

#include "int_matrix.h"

namespace cluster {
	class UnderlyingGraph : public IntMatrix {

		public:
			UnderlyingGraph(const IntMatrix&);

	};
}
namespace std {
	/* Add hash function to the std::hash struct. */
	template <>
	struct hash<cluster::UnderlyingGraph> {
		size_t operator()(const cluster::UnderlyingGraph &x) const {
			return x.hash();
		}
	};
	/**
	 * Annoyingly the standard shared_ptr hash function which passes the
	 * function to the object in the pointer doesn't seem to work.
	 */
	template <>
	struct hash<std::shared_ptr<cluster::UnderlyingGraph>> {
		size_t operator()(const std::shared_ptr<cluster::UnderlyingGraph> &x)
				const {
			return x->hash();
		}
	};
	/* Add equals function to std::equal_to */
	template<>
	struct equal_to<cluster::UnderlyingGraph> {
		bool operator()(const cluster::UnderlyingGraph &lhs,
		                const cluster::UnderlyingGraph &rhs) const {
			return lhs.equals(rhs);
		}
	};
	template<>
	struct equal_to<std::shared_ptr<cluster::UnderlyingGraph>> {
		bool operator()(const std::shared_ptr<cluster::UnderlyingGraph> &lhs,
		                const std::shared_ptr<cluster::UnderlyingGraph> &rhs) const {
			return lhs->equals(*rhs);
		}
	};
}

