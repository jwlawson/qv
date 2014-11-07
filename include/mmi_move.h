/**
 * mmi_move.h
 *
 * Header for the MMIMove class
 *
 * MMIMove represents a move that preserves the MMI-ness of a quiver.
 *
 * The move must match a subquiver of the provided quiver, then the move itself
 * consists of swapping this subquiver for a different one.
 *
 * There can be further constraints to when the move can apply to a quiver.
 * These are restrictions on the shape of the subquivers attached to the move's
 * subquiver.
 */
#pragma once

#include "equiv_quiver_matrix.h"

namespace cluster {

	class MMIMove {

		public:
			/**
			 * Create an MMIMove which switches the submatrix mata with the submatrix
			 * matb. These submatrices must be attached to the main quiver by only
			 * thoses nodes contained in the vector connections.
			 *
			 * The matrices are assumed to be the same size. Problems could occur if
			 * differently sized matrices are used.
			 */
			MMIMove(IntMatrix& mata, IntMatrix& matb, std::vector<int> connections);
			/**
			 * Apply the move to the provided matrix.
			 * The matrix must contain one of the move's submatrices, and the rows
			 * which make up this submatrix must be given in the vector provided.
			 */
			void move(IntMatrix& matrix, std::vector<int> submatrix);
			/**
			 * Find all submatrices of the matrix to which this move can be applied.
			 *
			 * The submatrices are returned in a vector, each submatrix represented by
			 * a vector of which rows in the initial matrix make up the submatrix.
			 */
			std::vector<std::vector<int>> applicable_submatrices(IntMatrix& matrix);

		private:
			/** First submatrix in move. */
			EquivQuiverMatrix mata_;
			/** Second submatrix in move. */
			EquivQuiverMatrix matb_;
			/** Size of the move matrices. */
			int size_;
			/** 
			 * List of rows of submatrices which connect to the main quiver.
			 *
			 * The move will only apply to a quiver if one of the matrices is embedded
			 * in the matrix, and the only connections between the quiver and the
			 * submatrix occur at the nodes in the list.
			 */
			std::vector<int> conn_;

			/** Check whether the submatrix matches the required connections. */
			bool check_connections(IntMatrix& matrix, std::vector<int> submatrix);
			
	};
}

