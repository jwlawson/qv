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
		private:
			typedef std::shared_ptr<std::istream> StreamPtr;

		public:
			/**
			 * Create an empty iterator.
			 */
			StreamIterator();
			/**
			 * Create a new iterator to extract all matrices from the specified input
			 * stream.
			 * @param stream Stream containing matrices
			 */
			StreamIterator(std::istream& stream);
			/**
			 * Get the next matrix from the input stream.
			 * @return The next matrix
			 */
			std::shared_ptr<T> next();
			/**
			 * Check whether the iterator will return a valid matrix when next() is
			 * called.
			 * @return true if next() will return a valid matrix
			 */
			bool has_next();
			StreamIterator& operator=(const StreamIterator& iter) = default;

		private:
			/** True if next() will return a valid matrix. */
			bool has_next_;
			/** String buffer to store the current matrix string in. */
			std::string str_;
			/** Pointer to the input stream to extract matrices from. */
			StreamPtr input_;

			/**
			 * Deleter for the StreamPtr. As the pointer is always made from
			 * references to input streams they should not be deleted after the
			 * shared_ptr goes out of scope. This deleter actually does nothing when
			 * the shared_ptr determines that it should be deleted.
			 */
			struct NullDeleter {
				void operator()(const void *const) const {}
			};
	};
}

