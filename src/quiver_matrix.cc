/*
 * quiver_matrix.h
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
 *
 * QuiverMatrix implementation.
 *
 * Most actual implementation is for the template methods and so in the header
 * file.
 */
#include "quiver_matrix.h"

namespace cluster {
	QuiverMatrix::QuiverMatrix() : IntMatrix() {}
	
	QuiverMatrix::QuiverMatrix(const int rows, const int cols)
		: IntMatrix(rows, cols) {}
	
	QuiverMatrix::QuiverMatrix(const int rows, const int cols, const int values[])
		: IntMatrix(rows, cols, values) {}
	
	QuiverMatrix::QuiverMatrix(const QuiverMatrix &mat)
		: IntMatrix(mat) {}
	
	QuiverMatrix::QuiverMatrix(IntMatrix matrix) : IntMatrix(matrix) {}
	
	QuiverMatrix::QuiverMatrix(QuiverMatrix &&mat) : IntMatrix(mat) {}

	QuiverMatrix::QuiverMatrix(std::string str) : IntMatrix(str) {}

	QuiverMatrix &QuiverMatrix::operator=(QuiverMatrix mat) {
		IntMatrix::operator=(mat);
		return *this;
	}
	std::vector<int> QuiverMatrix::k_row_;
	std::vector<int> QuiverMatrix::k_col_;

	bool QuiverMatrix::is_infinite() const {
		for (int i = 0; i < num_rows_ * num_cols_; i++) {
			int val = data_[i];
			if (val >= 3 || val <= -3) {
				return true;
			}
		}
		return false;
	}

}
