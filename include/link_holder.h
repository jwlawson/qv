/*
 * link_holder.h
 */
#pragma once
#include <vector>
#include <memory>

namespace cluster {
	template<class T>
	class	LinkHolder {

	 public:
		LinkHolder();
		LinkHolder(const int k);
		~LinkHolder();
		void size(const int size);
		void matrix(std::shared_ptr<T> mat);
		std::shared_ptr<T> matrix() const;
		void link(const int k);
		bool has_link(const int k) const;
		bool is_complete() const;

	 private:
		std::shared_ptr<T> matrix_;
		std::vector<bool> links_;
		int size_;

	};
}

