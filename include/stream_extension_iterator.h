/**
 * stream_extension_iterator.h
 *
 * Headers for an iterator which takes an input stream of matrices and provides
 * all possible extension of those matrices.
 */
#pragma once

#include "extension_iterator.h"
#include "stream_iterator.h"

namespace cluster {
	template<class T>
	class StreamExtIterator {
		private:
			typedef std::shared_ptr<T> MatrixPtr;
		public:
			StreamExtIterator();
			StreamExtIterator(std::istream& istream);

			MatrixPtr next();
			bool has_next();
			void progress(bool prog);

		private:
			StreamIterator<T> stream_iter_;
			ExtensionIterator<T> ext_iter_;
			bool progress_;
	};
}

