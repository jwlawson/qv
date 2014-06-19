/*
 * stream_iterator.h
 */
#pragma once

#include <istream>
#include <memory>
#include <string>

namespace cluster {
	template<class T>
	class StreamIterator{

		public:
			StreamIterator();
			StreamIterator(std::istream& stream);
			std::shared_ptr<T> next();
			bool has_next();

		private:
			bool has_next_;
			std::string str_;
			std::istream& input_;

	};
}

