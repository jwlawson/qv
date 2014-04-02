/*
 * mutation_class_size_test.cc
 */

#include "gtest/gtest.h"
#include "mutation_class_size.h"
#include "dynkin.h"

namespace cluster {

	TEST(MutationClassSize, A3) {
		QuiverMatrix m = dynkin::A3;
		int exp = 14;
		int result = classsize::Size(m);

		EXPECT_EQ(exp, result);
	}

	TEST(MutationClassSize, A4) {
		QuiverMatrix m = dynkin::A4;
		int exp = 144;
		int result = classsize::Size(m);

		EXPECT_EQ(exp, result);
	}

	TEST(MutationClassSize, D4) {
		QuiverMatrix m = dynkin::D4;
		int exp = 50;
		int result = classsize::Size(m);

		EXPECT_EQ(exp, result);
	}

	TEST(MutationClassSize, D5) {
		QuiverMatrix m = dynkin::D5;
		int exp = 2184;
		int res = classsize::Size(m);

		EXPECT_EQ(exp, res);
	}

	TEST(MutationClassSize, E6) {
		QuiverMatrix m = dynkin::E6;
		int exp = 42840;
		int res = classsize::Size(m);

		EXPECT_EQ(exp, res);
	}

	TEST(MutationClassSize, Infinite) {
		int v[] = {0, 1, 0, 0, -1, 0, 1, 1, 0, -1, 0, 1, 0, -1, -1, 0};
		QuiverMatrix m(4, 4, v);
		int exp = -1;
		int res = classsize::Size(m);

		EXPECT_EQ(exp, res);
	}
	/* A7 takes 23 seconds to calculate on my machine. Not including this test
	 * as I prefer the test to be quicker than that.
	 */
}


