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
			/**
			 * Create a new Loader which will start with the specified matrix and
			 * check all possible moves in the supplied vector of moves.
			 * @param matrix Pointer to the initial matrix.
			 * @param moves Vector of moves to check.
			 */
			MoveClassLoader(const MPtr matrix, const std::vector<MovePtr>& moves);
			/**
			 * Check whether the next call to next() will be valid.
			 * @return true if next() will return a valid matrix.
			 */
			bool has_next();
			/**
			 * Get the next matrix from the Move class. This will compute any matrices
			 * which can be transformed from the returned matrix and add them to the
			 * queue to be returned in subsequent calls.
			 */
			MPtr next();
			/**
			 * Get the depth at which the previously returned matrix was found.
			 */
			int depth();

		private:
			/** Size of the matrix the moves are being applied to. */
			const int size_;
			/** Vector of the moves to check and apply. */
			const std::vector<MovePtr>& moves_;
			/** Queue of matrices to check moves against. */
			Queue queue_;
			/**
			 * Map of all prevously seen matrices. Each matrix is mapped to the depth
			 * at which it was found.
			 */
			Map map_;
			/** Depth that the last returned matrix was computed at. */
			int depth_;
	};
}

