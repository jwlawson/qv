/*
 * cycle.h
 *
 * Contains the class Cycle which provides a wrapper to hold a cycle that
 * appears in a quiver.
 */

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
		bool equals(const Cycle& rhs);

		/**
		 * Check whether the cycle contains the specified vertex.
		 * @param value Vertex index to check
		 * @return true if vertex is in the cycle
		 */
		bool contains(const int value);

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
