/*
 * class_ext_iterator_test.cc
 */
#include "class_ext_iterator.h"

#include "gtest/gtest.h"

namespace cluster {

	TEST(ClassExtIter, Twos) {
		std::string str = "{ { 0 2 -2 } { -2 0 2 } { 2 -2 0 } }";
		QuiverMatrix mat(str);

		EquivMutClassExtIterator iter(mat);

		int count = 0;
		while(iter.has_next() ) {
			count++;
			iter.next();
		}
		EXPECT_EQ(count, 125);
	}

}

