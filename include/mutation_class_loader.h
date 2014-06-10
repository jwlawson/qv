/**
 * mutation_class_loader.h
 *
 * Contains the MutationClassLoader class headers.
 *
 * MutationClassLoader is an iterator over all elements in the mutation class of a
 * seed matrix. If any matrix which is calculated is found to be infinite then
 * the special value MutationClassLoader::INFINITE is returned by 
 * MutationClassLoader::next() instead of the next matrix.
 */
#pragma once

#include <deque>
#include <memory>
#include <unordered_map>

#include "link_holder.h"

namespace cluster {
	template<class T>
	class MutationClassLoader {

	 public:
		/**
		 * Matrix which is returned if the mutation class is found to be infinite.
		 */
		const static T INFINITE;

		/**
		 * Create a new MutationClassLoader starting from the specified inital matrix.
		 *
		 * The class provides an iterator-like interface to go through all matrices
		 * in the mutation class.
		 * @param initial Matrix to seed the mutation class
		 */
		MutationClassLoader(const T& initial);
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
		 * Get a pointer to the next unique matrix in the mutation class.
		 *
		 * This is the same as next(), but a pointer is returned rather than an
		 * object.
		 */
		std::shared_ptr<T> next_ptr();
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
		/**
		 * Initial matrix used to seed the mutation class.
		 */
		std::shared_ptr<T> matrix_;
		/**
		 * Size of the initial matrix. Assumes that the matrix is square, so is
		 * usually matrix_.num_rows().
		 */
		int size_;
		/**
		 * Map used to store the matrices in the mutation class which have not 
		 * yet been handled. TEach matrix is a key to the LinkHolder which stores
		 * information about which mutations of the matrix have been seen.
		 */
		std::unordered_map<std::shared_ptr<T>, LinkHolder<T>> map_;

		/**
		 * Handle a matrix which has been seen before.
		 *
		 * The matrix was attained by a mutation from the previous at the specified
		 * vertex.
		 * @param matrix Matrix which was attained and has been seen before
		 * @param previous Matrix which was mutated
		 * @param vertex Vertex at which previous was mutated to give matrix
		 */
		virtual void seen_matrix(std::shared_ptr<T>matrix,
														 std::shared_ptr<T>previous,
														 const int vertex);
		/**
		 * Handle a matrix which has not been seen before.
		 *
		 * The matrix was attained by a mutation from the previous at the specified
		 * vertex.
		 * @param matrix Matrix which was attained and has been seen before
		 * @param previous Matrix which was mutated
		 * @param vertex Vertex at which previous was mutated to give matrix
		 */
		virtual void unseen_matrix(std::shared_ptr<T> matrix, 
															 std::shared_ptr<T> previous,
		                           const int vertex);
		/**
		 * Check whether the specified matrix has been seen before.
		 * @param matrix Matrix to check
		 * @return true if the matrix has been seen
		 */
		virtual bool have_seen(std::shared_ptr<T> matrix);

	 private:
		/**
		 * True if more matrixces should be calculated each time a new matrix is
		 * taken from the iterator.
		 */
		bool should_calc_;
		/**
		 * Queue of unhandled matrices.
		 */
		std::deque<std::shared_ptr<T>> queue_;
		/**
		 * Compute all mutations of the matrix and add to the map and queue if the
		 * new matrices should be.
		 * @param The matrix to mutate at all vertices
		 * @return true if all mutations handled. false is returned if an infinite
		 * matrix is found.
		 */
		bool compute_mutations(std::shared_ptr<T> matrix);
		/**
		 * Check whether the matrix is complete, that is whether all possible
		 * mutations have been seen.
		 * @param Matrix to check
		 * @return true if the matrix is complete
		 */
		bool is_complete(std::shared_ptr<T> matrix);
		/**
		 * Remove the matrix from the map.
		 * @param Matrix to remove
		 */
		void remove_handled(std::shared_ptr<T> matrix);
		/**
		 * Remove the matrix from the map.
		 * @param Matrix to remove
		 */
		void remove_complete(std::shared_ptr<T> matrix);
		/**
		 * Check whether to mutate at the specified vertex.
		 * @param matrix Matrix to check against
		 * @param i Index of the vertex
		 * @return true if should mutate
		 */
		bool mutate_at(std::shared_ptr<T> matrix, const int i);
	};
}

