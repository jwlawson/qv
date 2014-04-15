/*
 * submatrix_iterator.h
 */
#pragma once

#include <memory>

namespace cluster {
	template<class T>
	class SubmatrixIterator {

	 public:
		SubmatrixIterator();
		SubmatrixIterator(const T& initial);

		std::shared_ptr<T> next();
		bool has_next();

	 private:
		T matrix_;
		int count_;
		int max_;

	};
}

