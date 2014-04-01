/*
 * fast_infinite_test.cc
 */

#include "gtest/gtest.h"
#include "dynkin.h"
#include "fast_infinite.h"

namespace cluster {

	TEST(FastInfinite, Call) {
		QuiverMatrix matrix = dynkin::A5;
		EXPECT_FALSE(fastinf::is_infinite(matrix));
	}

	TEST(FastInfinite, Inf) {
		int v[] = { 0, 1, 0, 0, -1, 0, 1, 1, 0, -1, 0, 1, 0, -1, -1, 0};
		QuiverMatrix mat(4, 4, v);
		EXPECT_FALSE(fastinf::is_infinite(mat));
	}

	TEST(FastInfinite, Infinite5) {
		int v[] = { 0, -1, 1, 0, 2, 1, 0, -1, 0, -2, -1, 1, 0, 1, 2, 0, 0, -1, 0, 0, -2,
				2, -2, 0, 0};
		QuiverMatrix mat(5, 5, v);
		EXPECT_FALSE(fastinf::is_infinite(mat));
	}
}
