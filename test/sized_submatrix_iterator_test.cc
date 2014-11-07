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
		n = i.next();
		EXPECT_EQ(n.num_rows(), 1);
		EXPECT_EQ(n.num_cols(), 1);
		EXPECT_EQ(n.get(0,0), 0);

		std::vector<int> rows = i.get_rows();
		ASSERT_EQ(rows.size(), 1);
		EXPECT_EQ(rows[0], 0);

		n = i.next();
		EXPECT_EQ(n.num_rows(), 1);
		EXPECT_EQ(n.num_cols(), 1);
		EXPECT_EQ(n.get(0,0), 3);

		rows = i.get_rows();
		ASSERT_EQ(rows.size(), 1);
		EXPECT_EQ(rows[0], 1);

		EXPECT_FALSE(i.has_next());
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
		EXPECT_EQ(n.get(1,0), 3);
		EXPECT_EQ(n.get(0,1), 1);
		EXPECT_EQ(n.get(1,1), 4);

		ASSERT_TRUE(i.has_next());

		n = i.next();
		EXPECT_EQ(n.num_rows(), 2);
		EXPECT_EQ(n.num_cols(), 2);
		EXPECT_EQ(n.get(0,0), 0);
		EXPECT_EQ(n.get(0,1), 2);
		EXPECT_EQ(n.get(1,0), 6);
		EXPECT_EQ(n.get(1,1), 8);
		ASSERT_TRUE(i.has_next());

		n = i.next();
		EXPECT_EQ(n.num_rows(), 2);
		EXPECT_EQ(n.num_cols(), 2);
		EXPECT_EQ(n.get(0,0), 4);
		EXPECT_EQ(n.get(0,1), 5);
		EXPECT_EQ(n.get(1,0), 7);
		EXPECT_EQ(n.get(1,1), 8);

		ASSERT_FALSE(i.has_next());
	}

	TEST(SizedSubmatrixIter, FullSubmatrixReturnsSelf) {
		int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		IntMatrix m(3,3,v);

		SizedSubmatrixIterator i(3, m);

		ASSERT_TRUE(i.has_next());
		IntMatrix n = i.next();
		ASSERT_TRUE(m.equals(n));

		ASSERT_FALSE(i.has_next());
	}
		
}

