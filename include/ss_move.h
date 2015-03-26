/**
 * ss_move.h
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
 * Sink-source MMI move. Check whether a matrix conatinas any applicable
 * vertices and provide a method to carry out the move, without performing a
 * full mutation on the matrix.
 */
#pragma once

#include <memory>

#include "int_matrix.h"

namespace cluster {

class SSMove {
	private:
		typedef std::shared_ptr<IntMatrix> MatrixPtr;
	public:
		struct Applicable {
			Applicable(int row, MatrixPtr ptr) : row_(row), matrix_(ptr) {}
			int row_;
			MatrixPtr matrix_;
		};
		/**
		 * Check whether the supplied matrix contains any sink or source vertices.
		 * If it does, then a non-empty vector of Applicables is returned detailing
		 * each possible sink-source mutation.
		 */
		std::vector<Applicable> applicable_submatrices(const IntMatrix& m) const;
		std::vector<Applicable> applicable_submatrices(MatrixPtr m) const;
		/** Perform the applicable sink-source mutation. */
		void move(const Applicable& app, IntMatrix& result) const;
};
}

