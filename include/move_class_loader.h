/**
 * move_class_loader.h
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
 * This is an iterator which progressively calculates the entire class of
 * quivers which can be obtained by successively applying moves to an initial
 * quiver.
 *
 * The Loader requires a list of all moves to apply and an initial matrix to
 * apply them to.
 */
#pragma once

#include <map>
#include <memory>
#include <unordered_map>

#include "equiv_quiver_matrix.h"
#include "mmi_move.h"
#include "ss_move.h"

namespace cluster {
	class MoveClassLoader {
		public:
			struct Depth {
				Depth() : moves_(0), sinksource_(0) {}
				Depth(int moves, int ss) : moves_(moves), sinksource_(ss) {}
				int moves_;
				int sinksource_;
				/* https://gcc.gnu.org/onlinedocs/libstdc++/manual/pairs.html */
				bool operator<(const Depth& d) const {
					return moves_ < d.moves_ || 
						( !(d.moves_ < moves_) && sinksource_ < d.sinksource_);
				}
			};
		private:
			typedef EquivQuiverMatrix M;
			typedef std::shared_ptr<M> MPtr;
			typedef std::shared_ptr<MMIMove> MovePtr;
			typedef std::multimap<Depth, MPtr> Queue;
			typedef std::unordered_map<MPtr, Depth> Map;
		public:
			typedef std::vector<MovePtr> MoveVec;
			/**
			 * Create a new Loader which will start with the specified matrix and
			 * check all possible moves in the supplied vector of moves.
			 * @param matrix Pointer to the initial matrix.
			 * @param moves Vector of moves to check.
			 */
			MoveClassLoader(const MPtr matrix, const MoveVec & moves,
					bool sinksource = true);
			MoveClassLoader(MoveClassLoader&) = delete;
			MoveClassLoader(MoveClassLoader&&) = delete;
			MoveClassLoader& operator=(MoveClassLoader&) = delete;
			MoveClassLoader& operator=(MoveClassLoader&&) = delete;
			/**
			 * Check whether the next call to next() will be valid.
			 * @return true if next() will return a valid matrix.
			 */
			bool has_next() {
				return !queue_.empty();
			}
			/**
			 * Get the next matrix from the Move class. This will compute any matrices
			 * which can be transformed from the returned matrix and add them to the
			 * queue to be returned in subsequent calls.
			 */
			MPtr next();
			/**
			 * Get the depth at which the previously returned matrix was found.
			 */
			Depth depth() {
				return depth_;
			}

		private:
			/** Size of the matrix the moves are being applied to. */
			const int size_;
			/** Vector of the moves to check and apply. */
			const MoveVec & moves_;
			/** Sink source move. */
			const SSMove ssmove_;
			/** Queue of matrices to check moves against. */
			Queue queue_;
			/**
			 * Map of all prevously seen matrices. Each matrix is mapped to the depth
			 * at which it was found.
			 */
			Map map_;
			/** Depth that the last returned matrix was computed at. */
			Depth depth_;
			/** True if should use sink source moves. */
			const bool sinksource_;
	};
}

