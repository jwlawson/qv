/*
 * extension_iterator.h
 *
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
			T next();
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
			int index_;
			/** Max value that index can take while the extensions are valid. */
			int max_;
		
			/**
			 * Calculate x to the power p.
			 */
			static int ipow(int x, int p);
	};

}

