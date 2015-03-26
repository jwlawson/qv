/**
 * stream_extension_iterator.h
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

