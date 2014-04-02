/*
 * submatrix_iterator.h
 */

namespace cluster {
	template<class T>
	class SubmatrixIterator {

	 public:
		SubmatrixIterator(const T initial);

		T next();
		bool has_next();

	 private:
		T matrix_;
		int count_;
		int max_;

	};
}

