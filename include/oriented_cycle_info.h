/*
 * oriented_cycle_info.h
 */
#pragma once

#include "equiv_quiver_matrix.h"

namespace cluster {

	class OrientedCycleInfo {

		public:
			OrientedCycleInfo(const QuiverMatrix& matrix);

			bool equals(const OrientedCycleInfo& cycle) const;

			std::size_t hash() const;

		private:
			EquivQuiverMatrix matrix_;
	};
}

namespace std {
	/* Add hash function to the std::hash struct. */
	template <>
	struct hash<cluster::OrientedCycleInfo> {
		size_t operator()(const cluster::OrientedCycleInfo &x) const {
			return x.hash();
		}
	};
	template <>
	struct hash<std::shared_ptr<cluster::OrientedCycleInfo>> {
		size_t operator()(const std::shared_ptr<cluster::OrientedCycleInfo> &x)
				const {
			return x->hash();
		}
	};
	/* Add equals function to std::equal_to */
	template<>
	struct equal_to<cluster::OrientedCycleInfo> {
		bool operator()(const cluster::OrientedCycleInfo &lhs,
		                const cluster::OrientedCycleInfo &rhs) const {
			return lhs.equals(rhs);
		}
	};
	template<>
	struct equal_to<std::shared_ptr<cluster::OrientedCycleInfo>> {
		bool operator()(const std::shared_ptr<cluster::OrientedCycleInfo> &lhs,
		                const std::shared_ptr<cluster::OrientedCycleInfo> &rhs) const {
			return lhs->equals(*rhs);
		}
	};
}

