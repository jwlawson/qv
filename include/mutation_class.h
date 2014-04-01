#include <unordered_map>
#include <deque>
#include "link_holder.h"

namespace cluster {
	template<class T>
	class MutationClass {

	 public:
		MutationClass();
		MutationClass(const T initial);
		const T next();
		void calc_more(bool calc);
		
	 protected:
		std::unordered_map<T, LinkHolder<T>> map_;
		std::deque<T> queue_;
		
		bool is_complete(const T matrix);
		void remove_handled(T matrix);
		void remove_complete(T matrix);
		void seen_matrix(T matrix, const int vertex);
		void unseen_matrix(T matrix, T previous, const int vertex);
		bool have_seen(const T matrix);

		private:
			bool mutate_at(const T matrix, const int i);
	};
}

