/**
 * stream_extension_iterator_test.cc
 */
#include "stream_extension_iterator.h"

#include "gtest/gtest.h"

#include "quiver_matrix.h"

namespace cluster {

	TEST(StreamExtIterator, Default) {
		StreamExtIterator<QuiverMatrix> iter;

		EXPECT_FALSE(iter.has_next());
	}

	TEST(StreamExtIterator, Simple) {
		std::stringstream ss;
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix mat(3,3,v);
		ss << mat;

		StreamExtIterator<QuiverMatrix> iter(ss);

		ASSERT_TRUE(iter.has_next());

		int w[] = {1,2,3,2,4,5,6,2,7,8,9,2,-2,-2,-2,0};
		QuiverMatrix exp(4, 4, w);
		std::shared_ptr<QuiverMatrix> n = iter.next();
		EXPECT_TRUE(exp.equals(*n));
		EXPECT_TRUE(iter.has_next());
	}

	TEST(StreamExtIterator, Count) {
		std::stringstream ss;
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix mat(3,3,v);
		ss << mat;

		StreamExtIterator<QuiverMatrix> iter(ss);
		int count = 0;
		while(iter.has_next()){
			iter.next();
			count++;
		}
		EXPECT_TRUE(count == 125);
	}
}

