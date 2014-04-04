/*
 * equiv_mutation_class_size.h
 *
 * Contains method to find the size of a matrix mutation class up to permuting
 * the rows and columns in the matrix.
 */
#include "equiv_mutation_class.h"
#include "equiv_quiver_matrix.h"

namespace cluster {
	namespace equivsize {
		/**
		 * Find the size of the mutation class, up to permuting the vertices of the
		 * matrices. 
		 *
		 * Checking for permutations takes longer than just checking equality of
		 * the matrix, so for small cases this can be slower than checking the
		 * whole mutation class. However for larger matrices the mutation classes
		 * are ususally very large (think many millions) and so the program can
		 * easily run out of memory computing them all. This check however will
		 * not.
		 *
		 * @param matrix Matrix to sed the mutation class with
		 * @return Size of the mutation class
		 */
		int Size(const EquivQuiverMatrix& matrix);
	}
}
