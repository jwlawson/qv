/*
 * cycle_test.cc
 */

#include "cycle.h"

#include "gtest/gtest.h"

namespace cluster {

	TEST(Cycle, Contains) {
		std::vector<int> v = {0, 1, 2, 3};
		Cycle res(v);

		EXPECT_TRUE(res.contains(0));
		EXPECT_TRUE(res.contains(1));
		EXPECT_FALSE(res.contains(4));
		EXPECT_FALSE(res.contains(-1));
	}

	TEST(Cycle, SameEquals) {
		std::vector<int> v1 = {0, 1, 2};
		
		Cycle a(v1);
		Cycle b(v1);

		EXPECT_TRUE(a.equals(b));
	}

	TEST(Cycle, PermEquals) {
		std::vector<int> v1 = {0, 1, 2};
		std::vector<int> v2 = {1, 2, 0};

		Cycle a(v1);
		Cycle b(v2);

		EXPECT_TRUE(a.equals(b));
	}

	TEST(Cycle, NotEqual) {
		std::vector<int> v1 = {1, 2, 3};
		std::vector<int> v2 = {0, 1, 2, 3};

		Cycle a(v1);
		Cycle b(v2);

		EXPECT_FALSE(a.equals(b));
	}

	TEST(Cycle, NotEqualPerm) {
		std::vector<int> v1 = {1, 2, 3};
		std::vector<int> v2 = {2, 1, 3};

		Cycle a(v1);
		Cycle b(v2);

		EXPECT_FALSE(a.equals(b));
	}

	TEST(Cycle, PermutationEquals) {
		std::vector<int> v1 = {0, 1, 2};
		std::vector<int> v2 = {1, 2, 3};
		std::vector<int> map = {3, 0, 1, 2};

		Cycle a(v1);
		Cycle b(v2);

		bool res = a.equals(b, map);
		EXPECT_TRUE(res);
	}
}

