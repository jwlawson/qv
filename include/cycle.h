/*
 * cycle.h
 *
 * Contains the class Cycle which provides a wrapper to hold a cycle that
 * appears in a quiver.
 */
#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace cluster {

	class Cycle {

		public:
		/**
		 * Default constructor creates an empty cycle.
		 */
		Cycle();

		/**
		 * Create a cycle with the provided values. The values will be cyclically
		 * permuted so that the smallest value is the first one.
		 * @param vec Vector of vertex labels which form the cycle
		 */
		Cycle(const std::vector<int>& vec);

		/**
		 * Check whether this cycle is equal to the one provided.
		 * @param rhs Cycle to check if equal to
		 * @return true if equal
		 */
		bool equals(const Cycle& rhs) const;

		/**
		 * Hash the cycle and return the hashcode.
		 * @return The hashcode.
		 */
		std::size_t hash() const;

		/**
		 * Check whether the cycle contains the specified vertex.
		 * @param value Vertex index to check
		 * @return true if vertex is in the cycle
		 */
		bool contains(const int value) const;

		/**
		 * Need to overload == to allow unordered_sets of cycles to be compared.
		 */
		friend bool operator ==(const Cycle& lhs, const Cycle& rhs) {
			return lhs.equals(rhs);
		}

		private:
		/**
		 * Container to hold the cycle information.
		 */
		std::vector<int> cycle_;

		/**
		 * Find the index in the vector of the smallest value stored in the vector.
		 * @param vec Vector to check
		 * @return Index of the smallest value
		 */
		static std::size_t smallest_index(const std::vector<int>& vec);

	};
}

namespace std {
	/* Add hash function to the std::hash struct. */
	template <>
	struct hash<cluster::Cycle> {
		size_t operator()(const cluster::Cycle &x) const {
			return x.hash();
		}
	};
	/* Add equals function to std::equal_to */
	template<>
	struct equal_to<cluster::Cycle> {
		bool operator()(const cluster::Cycle &lhs,
		                const cluster::Cycle &rhs) const {
			return lhs.equals(rhs);
		}
	};
	template <>
	struct hash<std::shared_ptr<cluster::Cycle>> {
		size_t operator()(const std::shared_ptr<cluster::Cycle> &x)
				const {
			return x->hash();
		}
	};
	template<>
	struct equal_to<std::shared_ptr<cluster::Cycle>> {
		bool operator()(const std::shared_ptr<cluster::Cycle> &lhs,
		                const std::shared_ptr<cluster::Cycle> &rhs) const {
			return lhs->equals(*rhs);
		}
	};
}

