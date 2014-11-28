/**
 * move_class_loader.h
 *
 * Header file for the MoveClassLoader.
 *
 * This is an iterator which progressively calculates the entire class of
 * quivers which can be obtained by successively applying moves to an initial
 * quiver.
 *
 * The Loader requires a list of all moves to apply and an initial matrix to
 * apply them to.
 */
#pragma once

#include <deque>
#include <memory>
#include <unordered_map>

#include "equiv_quiver_matrix.h"
#include "mmi_move.h"

namespace cluster {
	class MoveClassLoader {
		private:
			typedef EquivQuiverMatrix M;
			typedef std::shared_ptr<M> MPtr;
			typedef std::shared_ptr<MMIMove> MovePtr;
			typedef std::deque<MPtr> Queue;
			typedef std::unordered_map<MPtr, int> Map;
		public:
			MoveClassLoader(const MPtr matrix, const std::vector<MovePtr>& moves);
			bool has_next();
			MPtr next();
			int depth();

		private:
			MPtr last_;
			const int size_;
			const std::vector<MovePtr>& moves_;
			Queue queue_;
			Map map_;
			int depth_;
	};
}

