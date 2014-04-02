#include "gtest/gtest.h"

#include "equiv_quiver_matrix.h"
#include "int_matrix.h"
#include "equivalence_checker.h"

#include <memory>

namespace cluster {
	TEST(EquivMatrix, SimpleTest) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		EXPECT_TRUE(m1.equals(m2));
		EXPECT_TRUE(m2.equals(m1));

		int v3[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v4[] = {0, -1, 0, 1, 0, -1, 0, 1, 0};
		EquivQuiverMatrix m3(3, 3, v3);
		EquivQuiverMatrix m4(3, 3, v4);

		EXPECT_TRUE(m3.equals(m4));
	}

	TEST(EquivMatrix, SameAreEqual) {
		int v1[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
		EquivQuiverMatrix m1(3, 3, v1);

		EXPECT_TRUE(m1.equals(m1));

		int v2[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
		EquivQuiverMatrix m2(3, 3, v2);

		EXPECT_TRUE(m1.equals(m2));
	}

	TEST(EquivMatrix, 4x4) {
		int v1[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
		EquivQuiverMatrix m1(4, 4, v1);
		EquivQuiverMatrix m2(4, 4, v1);

		EXPECT_TRUE(m1.equals(m2));
	}

	TEST(EquivMatrix, 4) {
		int v1[] = {0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1};
		int v2[] = {0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1};
		EquivQuiverMatrix m1(4, 4, v1);
		EquivQuiverMatrix m2(4, 4, v2);
		EXPECT_TRUE(m1.equals(m2));
	}

	TEST(EquivMatrix, NotEquivalent) {
		int v1[] = {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4};
		int v2[] = {5, 0, 0, 1, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4};
		EquivQuiverMatrix m1(4, 4, v1);
		EquivQuiverMatrix m2(4, 4, v2);
		EXPECT_FALSE(m1.equals(m2));
	}

	TEST(EquivMatrix, Quivers) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		EXPECT_TRUE(m1.equals(m2));
	}

	TEST(EquivMatrix, Checker) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		std::shared_ptr<EquivalenceChecker> ch = EquivalenceChecker::Get(3);
		EXPECT_TRUE(ch->are_equivalent(m1, m2));
	}

	TEST(EquivMatrix, Cache) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		std::shared_ptr<EquivalenceChecker> ch = EquivalenceChecker::Get(3);
		bool a = ch->are_equivalent(m1, m2);
		EXPECT_TRUE(a);

		int v3[] = {0, 1, 0, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix notEquiv(3, 3, v3);
		bool b = ch->are_equivalent(m1, notEquiv);
		bool c = ch->are_equivalent(m2, notEquiv);
		EXPECT_FALSE(b);
		EXPECT_FALSE(c);
	}

	TEST(EquivMatrix, 5x5) {

		int v1[] = {0, -1, 0, 0, 0, 1, 0, 0, -1, 1, 0, 0, 0, 1, -1, 0, 1, -1, 0,
		            0,	0, -1, 1, 0, 0
		           };
		int v2[] = {0, 0, 0, -1, 1, 0, 0, 1, 1, -1, 0, -1, 0, 0, 0, 1, -1, 0, 0,
		            0,	-1, 1, 0, 0, 0
		           };
		EquivQuiverMatrix m1(5, 5, v1);
		EquivQuiverMatrix m2(5, 5, v2);

		EXPECT_TRUE(m1.equals(m2));

		int v3[] = {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,	0};
		IntMatrix perm(5, 5, v3);
		IntMatrix res1(5, 5);
		IntMatrix res2(5, 5);
		EXPECT_TRUE(m1.mult_right(perm, res1).equals(m2.mult_left(perm, res2)));
	}

	TEST(EquivMatrix, stdequal) {
		int v1[] = {0, 1, 2, 3};
		EquivQuiverMatrix m1(2, 2, v1);
		EquivQuiverMatrix m2(2, 2, v1);

		std::equal_to<EquivQuiverMatrix> eq;

		EXPECT_TRUE(eq(m1, m2));

	}
	TEST(EquivMatrix, stdequalEquiv) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		std::equal_to<EquivQuiverMatrix> eq;

		EXPECT_TRUE(eq(m1, m2));
	}
}
