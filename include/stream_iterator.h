/*
 * stream_iterator.h
 */
#pragma once

// Need iostream for std::cin which is not in istream.
#include <iostream>
#include <memory>
#include <string>

namespace cluster {
	template<class T>
	class StreamIterator{

		public:
			StreamIterator(std::istream& stream = std::cin);
			std::shared_ptr<T> next();
			bool has_next();

		private:
			bool has_next_;
			std::string str_;
			std::istream& input_;

	};
}

