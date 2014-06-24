/**
 * stream_extension_iterator.cc
 */
#include "stream_extension_iterator.h"

#include <iostream>

#include "equiv_quiver_matrix.h"
#include "quiver_matrix.h"

namespace cluster {

	template<class T>
	StreamExtIterator<T>::StreamExtIterator()
		: stream_iter_(),
			ext_iter_(),
			progress_(false) {}

	template<class T>
	StreamExtIterator<T>::StreamExtIterator(std::istream& istream)
		: stream_iter_(istream),
			ext_iter_(),
			progress_(false) {}

	template<class T>
	std::shared_ptr<T> StreamExtIterator<T>::next() {
		if(!ext_iter_.has_next()){
			ext_iter_ = ExtensionIterator<T>(*stream_iter_.next());
			if(progress_){
				std::cerr << "*";
			}
		}
		return std::make_shared<T>(ext_iter_.next());
	}

	template<class T>
	bool StreamExtIterator<T>::has_next() {
		return ext_iter_.has_next() || stream_iter_.has_next();
	}

	template<class T>
	void StreamExtIterator<T>::progress(bool prog) {
		progress_ = prog;
	}

	template class StreamExtIterator<QuiverMatrix>;
	template class StreamExtIterator<EquivQuiverMatrix>;
}

