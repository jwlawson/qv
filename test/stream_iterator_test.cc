/*
 * stream_iterator_test.cc
 */
#include "stream_iterator.h"

#include <fstream>
#include <sstream>

#include "gtest/gtest.h"

#include "quiver_matrix.h"

namespace cluster {

	TEST(StreamIterator, First) {
		std::stringstream ss;
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix mat(3,3,v);
		ss << mat;

		StreamIterator<QuiverMatrix> iter(ss);

		ASSERT_TRUE(iter.has_next());
		EXPECT_TRUE(mat.equals(*(iter.next())));
		EXPECT_FALSE(iter.has_next());
	}

	TEST(StreamIterator, Empty) {
		std::ifstream file;
		file.open("test/empty");
		ASSERT_TRUE(file.is_open());

		StreamIterator<QuiverMatrix> iter(file);

		EXPECT_FALSE(iter.has_next());
	}
}

