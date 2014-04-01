#include <unordered_map>
#include <deque>
#include "link_holder.h"

namespace cluster {
	template<class T>
	class MutationClass {

	 public:
		const static T INFINITE;

		MutationClass();
		MutationClass(const T initial);
		/**
		 * Get the next unique matrix in the mutation class.
		 *
		 * The method will first compute the other matrices which are muitations
		 * of the next matrix, and so fill the queue. This means that the method
		 * will not be instant.
		 *
		 * The ordering of the matrices follows the order in which they were found
		 * and cannot be guaranteed.
		 *
		 * @return The next matrix in the mutation class.
		 */
		T next();
		/**
		 * Check whether the iterator has another matrix in the mutation class.
		 * @return true if there is another matrix
		 */
		bool has_next();
		/**
		 * Set whether more matrices should be calculated when the next matrix
		 * is accessed.
		 * @param calc true if want to calculate more matrices.
		 */
		void calc_more(const bool calc);

	 protected:
		T matrix_;
		bool should_calc_;
		int size_;
		std::unordered_map<T, LinkHolder<T>> map_;
		std::deque<T> queue_;

		/**
		 * Compute all mutations of the matrix and add to the map and queue if the
		 * new matrices should be.
		 * @param The matrix to mutate at all vertices
		 * @return true if all mutations handled. false is returned if an infinite
		 * matrix is found.
		 */
		bool compute_mutations(const T &matrix);
		bool is_complete(const T &matrix);
		void remove_handled(const T &matrix);
		void remove_complete(const T &matrix);
		void seen_matrix(const T &matrix, const T &previous, const int vertex);
		void unseen_matrix(const T &matrix, const T &previous, const int vertex);
		bool have_seen(const T &matrix);

	 private:
		bool mutate_at(const T &matrix, const int i);
	};
}

