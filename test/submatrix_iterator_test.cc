/*
 * submatrix_iterator_test.cc
 */
#include "submatrix_iterator.h"

#include "gtest/gtest.h"

#include "quiver_matrix.h"

namespace cluster {
	
	TEST(SubmatrixIter, First) {
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix m(3,3,v);

		int w[] = {5,6,8,9};
		QuiverMatrix exp(2,2,w);

		SubmatrixIterator<QuiverMatrix> iter(m);

		EXPECT_TRUE(exp.equals(*(iter.next())));
	}

	TEST(SubmatrixIter, Second) {
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix m(3,3,v);

		int e[] = {1,3,7,9};
		QuiverMatrix exp(2,2,e);

		SubmatrixIterator<QuiverMatrix> iter(m);
		iter.next();

		EXPECT_TRUE(exp.equals(*(iter.next())));
	}

	TEST( SubmatrixIter, Degenerate) {
		int v[] = {1,4,5,9,0,0,2,0,0};
		QuiverMatrix m(3,3,v);

		QuiverMatrix exp;

		SubmatrixIterator<QuiverMatrix> iter(m);

		EXPECT_TRUE(exp.equals(*(iter.next())));
	}
}
