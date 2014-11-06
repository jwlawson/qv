/**
 * sized_submatrix_iterator_test.cc
 */
#include "gtest/gtest.h"

#include "sized_submatrix_iterator.h"

namespace cluster {

	TEST(SizedSubmatrixIter, Single) {
		int v[] = {0, 1, 2, 3};
		IntMatrix m(2,2,v);

		SizedSubmatrixIterator i(1, m);
		ASSERT_TRUE(i.has_next());

		IntMatrix n;
		int c = 0;
		while(i.has_next()){
			n = i.next();
			EXPECT_EQ(n.num_rows(), 1);
			EXPECT_EQ(n.num_cols(), 1);
			EXPECT_EQ(n.get(0,0), c);
			c++;
		}
		EXPECT_EQ(c, 4);
	}

	TEST(SizedSubmatrixIter, TwosInThree) {
		int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		IntMatrix m(3,3,v);

		SizedSubmatrixIterator i(2,m);
		ASSERT_TRUE(i.has_next());

		IntMatrix n;
		n = i.next();
		EXPECT_EQ(n.num_rows(), 2);
		EXPECT_EQ(n.num_cols(), 2);
		EXPECT_EQ(n.get(0,0), 0);
		EXPECT_EQ(n.get(1,1), 1);

		ASSERT_TRUE(i.has_next());

		n = i.next();
		EXPECT_EQ(n.num_rows(), 2);
		EXPECT_EQ(n.num_cols(), 2);
		EXPECT_EQ(n.get(0,0), 4);
		EXPECT_EQ(n.get(1,1), 5);

		ASSERT_FALSE(i.has_next());
	}
		
}

