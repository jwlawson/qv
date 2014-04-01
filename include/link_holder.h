/*
 * link_holder.h
 */

#include <vector>

namespace cluster {
	template<class T>
	class	LinkHolder {

	 public:
		LinkHolder(const int k);
		~LinkHolder();
		void matrix(T mat);
		T matrix() const;
		void link(const int k);
		bool has_link(const int k) const;
		bool is_complete() const;

	 private:
		T matrix_;
		std::vector<bool> links_;

	};
}

