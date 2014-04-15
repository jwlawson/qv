/*
 * stream_submatrix_iterator_test.cc
 */
#include "stream_submatrix_iterator.h"

#include <sstream>

#include "gtest/gtest.h"

#include "quiver_matrix.h"

namespace cluster {

	TEST(StreamSubIterator, First) {
		std::stringstream ss;
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix mat(3,3,v);
		ss << mat;
		int w[] = {5,6,8,9};
		QuiverMatrix exp1(2,2,w);

		int x[] = {1,3,7,9};
		QuiverMatrix exp2(2,2,x);

		StreamSubIter<QuiverMatrix> iter(ss);

		ASSERT_TRUE(iter.has_next());
		EXPECT_TRUE(exp1.equals(*(iter.next())));
		EXPECT_TRUE(exp2.equals(*(iter.next())));
	}

	TEST(StreamSubIterator, Info) {
		std::stringstream ss;
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix mat(3,3,v);
		ss << mat;
		int w[] = {5,6,8,9};
		QuiverMatrix exp1(2,2,w);

		int x[] = {1,3,7,9};
		QuiverMatrix exp2(2,2,x);

		StreamSubIter<QuiverMatrix> iter(ss);

		ASSERT_TRUE(iter.has_next());

		StreamSubIter<QuiverMatrix>::MatrixSub res = iter.next_info();

		EXPECT_TRUE(exp1.equals(*res.submatrix));
		EXPECT_TRUE(mat.equals(*res.matrix));
	}
}

