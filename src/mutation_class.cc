/*
 * mutation_class.cc
 */

#include "quiver_matrix.h"
#include "equiv_quiver_matrix.h"
#include "mutation_class.h"

#include <iostream>

namespace cluster {

	template< class T>
	const T MutationClass<T>::INFINITE;

	template<class T>
	MutationClass<T>::MutationClass(const T& mat)
		: size_(mat.num_rows()),
		  map_(),
		  should_calc_(true),
		  queue_() {
		matrix_ = std::make_shared<T>(mat);
		map_[matrix_].matrix(matrix_);
		map_[matrix_].size(size_);
		queue_.push_back(matrix_);
		count_ = 0;
	}

	template<class T>
	T MutationClass<T>::next() {
		std::shared_ptr<T> result = queue_.front();
		queue_.pop_front();
		count_++;
		if (compute_mutations(result)) {
			return std::move(*result);
		} else {
			return INFINITE;
		}
	}

	template<class T>
	bool MutationClass<T>::has_next() {
		return !queue_.empty();
	}

	template<class T>
	void MutationClass<T>::calc_more(const bool calc) {
		should_calc_ = calc;
	}

	template<class T>
	bool MutationClass<T>::compute_mutations(std::shared_ptr<T> mat) {
		for (int i = 0; i < size_ && should_calc_; i++) {
			if (mutate_at(mat, i)) {
				if(count_ == 1459) std::cout<< "mutating at "<< i << std::endl;
				const std::shared_ptr<T> new_matrix =
					std::make_shared<T>(mat->num_rows(), mat->num_cols());
				mat->mutate(i, *new_matrix);
				if(count_ == 1459) std::cout<< "Got " << *new_matrix << new_matrix << std::endl;
				if (have_seen(new_matrix)) {
					if(count_ == 1459) {
						std::hash<T> thash;
						std::hash<std::shared_ptr<T>> phash;
						std::cout << thash(*new_matrix) << " / " << phash(new_matrix) << std::endl;
						auto found = map_.find(new_matrix);
						if(found == map_.end()) {
							std::cout << "Matrix not found" << std::endl;
						} else {
							std::cout << found->first << " mapped to " << found->second.matrix() << std::endl;
							std::cout << "Equal? " << (*new_matrix).equals(*found->first)<< std::endl;
							std::cout << *(found->first) << " mapped to " << std::endl;
							std::cout << thash(*found->first) << " / " << phash(found->first) << std::endl;
							int count = map_.count(new_matrix);
							std::cout << "Count: "<< count << new_matrix << *new_matrix << std::endl;
							std::cout << phash(new_matrix) << std::endl;
							for(auto& a : map_) {
								std::cout << phash(a.first) << std::endl;
								if(phash(new_matrix) == phash(a.first) ) {
									std::cout << "In map " << a.first << " " << *a.first << std::endl;
									std::cout << "Equal? " << (*new_matrix).equals(*a.first)<< std::endl;
								}
							}
							auto found1 = map_.find(new_matrix);
							if(found1 == map_.end()) {
								std::cout << "Not found ????" << std::endl;
							} else {
							std::cout << "Same? " << (found->first == found1->first)  << " " << found1->first << "->" << found1->second.matrix() << std::endl;
							int count2 = map_.count(new_matrix);
							std::cout << "Count: "<< count2 << std::endl;
							}
						}
					}
					seen_matrix(new_matrix, mat, i);
					if(count_ == 1459) std::cout<< "No!" << std::endl;
					if (is_complete(new_matrix)) {
						remove_complete(new_matrix);
					}
				} else {
					if (new_matrix->is_infinite()) {
						return false;
					}
					unseen_matrix(new_matrix, mat, i);
				}
			}
		}
		remove_handled(mat);
		return true;
	}

	template<class T>
	bool MutationClass<T>::is_complete(std::shared_ptr<T> matrix) {
		return map_[matrix].is_complete();
	}

	template<class T>
	void MutationClass<T>::remove_handled(std::shared_ptr<T> matrix) {
		map_.erase(matrix);
	}

	template<class T>
	void MutationClass<T>::remove_complete(std::shared_ptr<T> matrix) {
		map_.erase(matrix);
	}

	template<class T>
	void MutationClass<T>::seen_matrix(std::shared_ptr<T> matrix,
																		 std::shared_ptr<T> previous,
	                                   const int vertex) {
		map_[matrix].link(vertex);
		map_[previous].link(vertex);
	}

	template<class T>
	void MutationClass<T>::unseen_matrix(std::shared_ptr<T> matrix,
																			 std::shared_ptr<T> previous,
	                                     const int vertex) {
		queue_.push_back(matrix);
		map_[matrix].size(size_);
		map_[matrix].link(vertex);
		map_[matrix].matrix(matrix);
		map_[previous].link(vertex);
	}

	template<class T>
	bool MutationClass<T>::have_seen(std::shared_ptr<T> matrix) {
		return map_.count(matrix) > 0;
	}

	template<class T>
	bool MutationClass<T>::mutate_at(std::shared_ptr<T> matrix, const int i) {
		return map_.count(matrix) > 0 && !map_[matrix].has_link(i);
	}

	template class MutationClass<QuiverMatrix>;
	template class MutationClass<EquivQuiverMatrix>;
}

