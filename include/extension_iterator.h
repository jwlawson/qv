/*
 * extension_iterator.h
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
 * Contains the ExtensionIterator which iterates over all possible
 * skew-symmetric extensions of the seed matrix.
 */
#pragma once

namespace cluster {
	template<class T>
	class ExtensionIterator{

		public:
			ExtensionIterator();
			/**
			 * Create a new iterator with the specified seed matrix.
			 * @param matrix Matrix to extend in all possible ways
			 */
			ExtensionIterator(const T& matrix);
			/**
			 * Return the next matrix in the iterator.
			 */
			T const& next();
			/**
			 * Check if the iterator has another matrix to return.
			 * @return true if calling next will return a valid matrix
			 */
			bool has_next();

		private:
			/** Enlarged matrix copy of the original. */
			T matrix_;
			/** Size of the original matrix. */
			int size_;
			/** Index of the extension to return next. */
			long index_;
			/** Max value that index can take while the extensions are valid. */
			long max_;
		
			/**
			 * Calculate x to the power p.
			 */
			static long ipow(int x, int p);
	};

}

