/*
 * extension_iterator_test.cc
 */

#include "gtest/gtest.h"
#include "extension_iterator.h"
#include "quiver_matrix.h"

namespace cluster {

	TEST(ExtIter, First) {
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix m(3,3,v);

		int w[] = {1,2,3,2,4,5,6,2,7,8,9,2,-2,-2,-2,0};
		QuiverMatrix exp(4,4,w);

		ExtensionIterator<QuiverMatrix> iter(m);

		EXPECT_TRUE(exp.equals(iter.next()));
	}

	TEST(ExtIter, Second) {
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix m(3,3,v);

		int w[] = {1,2,3,1,4,5,6,2,7,8,9,2,-1,-2,-2,0};
		QuiverMatrix exp(4,4,w);

		ExtensionIterator<QuiverMatrix> iter(m);
		iter.next();

		EXPECT_TRUE(exp.equals(iter.next()));
	}
}

