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

#include <map>

#include "equiv_quiver_matrix.h"

namespace cluster {
namespace mmi_conn {

	typedef std::shared_ptr<const std::vector<int>> VectorPtr;
	typedef std::shared_ptr<IntMatrix> MatrixPtr;
	/**
	 * Struct containing all information about how a submatrix is embedded in a
	 * larger matrix.
	 */
	struct Submatrix {
		Submatrix(const MatrixPtr mat, const std::vector<int>&& sub,
				const std::vector<int>&& perm) : 
			matrix_(mat),
			submatrix_(std::make_shared<std::vector<int>>(sub)),
			perm_(std::make_shared<std::vector<int>>(perm)){}
		Submatrix(const MatrixPtr mat, const VectorPtr sub, const std::vector<int>&& perm)
			: matrix_(mat), submatrix_(sub),
			perm_(std::make_shared<std::vector<int>>(perm)){}
		const MatrixPtr matrix_;
		const VectorPtr submatrix_;
		const VectorPtr perm_;
	};
	/**
	 * Functor that returns true only if the arrows from the connection never
	 * return to the submatrix.
	 */
	struct Unconnected {
		bool operator()(const Submatrix& sub, int connection);
		private:
		std::vector<int> seen_;
		bool isUnconnected(const Submatrix& sub, int depth, int next);
	};
	struct Line {
		bool operator()(const Submatrix& sub, int connection) const;
		private:
		bool isLine(const Submatrix& sub, int next, int prev) const;
	};
	/**
	 * Functor that checks whether any arrows from the connection which eventually
	 * coincide with the submatrix again only return at the specified vertices.
	 * If the arrows never return to the matrix (or if there are no arrows from
	 * the connection) then this will also return true.
	 */
	struct ConnectedTo {
		ConnectedTo(int conn) : conn_({conn}), sub_conn_(1) {}
		ConnectedTo(int conn1, int conn2) : conn_({conn1, conn2}), sub_conn_(2) {}
		ConnectedTo(std::initializer_list<int> l) : conn_(l), sub_conn_(l.size()) {}
		bool operator()(const Submatrix& sub, int connection);
		private:
		std::vector<int> conn_;
		std::vector<int> sub_conn_;
		std::vector<int> seen_;
		bool isConnected(const Submatrix& sub, int depth, int next);
	};
	/**
	 * Functor that checks whether the submatrix has a line connecting the
	 * connection to the specified vertex.
	 */
	struct LineTo {
		LineTo(int conn) : conn_(conn) {}
		bool operator()(const Submatrix& sub, int connection);
		private:
		const int conn_;
		int sub_conn_;
		bool isLine(const Submatrix& sub, int next, int prev) const;
	};
	/**
	 * Functor that returns true if the connection has no arrows outside the
	 * submatrix.
	 */
	struct None {
		bool operator()(const Submatrix& sub, int conenction) const;
	};
}
class MMIMove {

	public:
		typedef std::function<bool(const mmi_conn::Submatrix&, int)> ConnReq;
		typedef std::map<int, ConnReq> Connections;
		typedef std::shared_ptr<const std::vector<int>> VectorPtr;
		typedef std::shared_ptr<IntMatrix> MatrixPtr;
		/**
		 * Struct containing all information about a submatrix which is applicable
		 * to this move. The match is not which move matrix is embedded in the
		 * submatrix, but rather the one that the move will take that to.
		 */
		struct Applicable {
			Applicable(const mmi_conn::Submatrix& sub, const IntMatrix& match)
				: matrix_(sub.matrix_),
					submatrix_(sub.submatrix_),
					perm_(sub.perm_),
					match_(match) {}
			const MatrixPtr matrix_;
			const VectorPtr submatrix_;
			const VectorPtr perm_;
			const IntMatrix& match_;
		};
		/**
		 * Create an MMIMove which switches the submatrix mata with the submatrix
		 * matb. These submatrices must be attached to the main quiver by only
		 * thoses nodes contained in the vector connections.
		 *
		 * The matrices are assumed to be the same size. Problems could occur if
		 * differently sized matrices are used.
		 *
		 * The default connection requirement is Unconnected, that is each subquiver
		 * at the connection is not connected to any other connection.
		 */
		MMIMove(const IntMatrix& mata, const IntMatrix& matb,
				const std::vector<int>& connections);
		/**
		 * Create an MMIMove which switches the submatrix mata with the submatrix
		 * matb. These submatrices must be attached to the main quiver by only
		 * thoses nodes contained in the vector connections.
		 *
		 * The matrices are assumed to be the same size. Problems could occur if
		 * differently sized matrices are used.
		 *
		 * The connections between the submatrix and the main quiver are given in
		 * the vector of ints, and the requirements for the shape of the quiver at
		 * that connection in the other vector.
		 */
		MMIMove(const IntMatrix& mata, const IntMatrix& matb,
				const std::vector<int>& conn, const std::vector<ConnReq>& req);
		/**
		 * Apply the move to the provided matrix.
		 * The matrix must contain one of the move's submatrices, and the rows
		 * which make up this submatrix must be given in the vector provided.
		 */
		void move(const Applicable& app, IntMatrix& result);
		/**
		 * Find all submatrices of the matrix to which this move can be applied.
		 *
		 * The submatrices are returned in a vector, each submatrix represented by
		 * a vector of which rows in the initial matrix make up the submatrix.
		 */
		std::vector<Applicable> applicable_submatrices(const IntMatrix& matrix);
		std::vector<Applicable> applicable_submatrices(const MatrixPtr matrix);

	private:
		/** First submatrix in move. */
		const EquivQuiverMatrix mata_;
		/** Second submatrix in move. */
		const EquivQuiverMatrix matb_;
		/** Size of the move matrices. */
		const int size_;
		/** 
		 * List of rows of submatrices which connect to the main quiver.
		 *
		 * The move will only apply to a quiver if one of the matrices is embedded
		 * in the matrix, and the only connections between the quiver and the
		 * submatrix occur at the nodes in the list.
		 */
		Connections conn_;

		/** Check whether the submatrix matches the required connections. */
		bool check_connections(const mmi_conn::Submatrix& submatrix) const;
};
}

