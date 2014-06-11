/*
 * skew_symmetric_iterator_test.cc
 */
#include "skew_symmetric_iterator.h"

#include "gtest/gtest.h"

namespace cluster {

	TEST(SkewSymIter, Size3) {
		SkewSymmetricIterator iter(3);

		ASSERT_TRUE(iter.has_next());

		std::string s = "{ { 0 -2 -2 } { 2 0 -2 } { 2 2 0 } }";
		EquivQuiverMatrix m(s);
		EXPECT_TRUE(m.equals(*iter.next()));
		
		EXPECT_TRUE(iter.has_next());
	}
}

