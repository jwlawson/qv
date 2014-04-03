/*
 * extension_iterator.h
 */

namespace cluster {
	template<class T>
	class ExtensionIterator{

		public:
			ExtensionIterator(const T& matrix);
			T next();
			bool has_next();

		private:
			T matrix_;
			int size_;
			int index_;
			int max_;
		
			static int ipow(int x, int p);
	};

}

