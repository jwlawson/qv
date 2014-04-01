#include "gtest/gtest.h"

#include "quiver_matrix.h"

namespace cluster {

	TEST(QuiverMatrix, Construct) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		QuiverMatrix mat(3, 3, v1);

		EXPECT_EQ(0, mat.get(0, 0));
		EXPECT_EQ(1, mat.get(0, 1));
	}

	TEST(QuiverMatrix, Mutate) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, -1, 0, 1, 0, 1, 0, -1, 0};
		QuiverMatrix mat(3, 3, v1);
		QuiverMatrix exp1(3, 3, v2);
		QuiverMatrix res(3, 3);

		EXPECT_TRUE(exp1.equals(mat.mutate(0, res)));
	}

	TEST(QuiverMatrix, Mutate2) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, -1, 1, 1, 0, -1, -1, 1, 0};
		QuiverMatrix mat(3, 3, v1);
		QuiverMatrix exp(3, 3, v2);
		QuiverMatrix res(3, 3);

		EXPECT_TRUE(exp.equals(mat.mutate(1, res)));
	}

	TEST(QuiverMatrix, Markov) {
		int v1[] = {0, 2, -2, -2, 0, 2, 2, -2, 0};
		int v2[] = {0, -2, 2, 2, 0, -2, -2, 2, 0};
		QuiverMatrix mat(3, 3, v1);
		QuiverMatrix exp(3, 3, v2);
		QuiverMatrix res(3, 3);

		EXPECT_TRUE(exp.equals(mat.mutate(0, res)));
		EXPECT_TRUE(exp.equals(mat.mutate(1, res)));
		EXPECT_TRUE(exp.equals(mat.mutate(2, res)));
	}

	TEST(QuiverMatrix, Enlarge) {
		int v1[] = {0, 2, -2, -2, 0, 2, 2, -2, 0};
		int v2[] = {0, 2, -2, 0, -2, 0, 2, 0, 2, -2, 0, 0, 0, 0, 0, 0};
		QuiverMatrix m(3, 3, v1);
		QuiverMatrix exp(4, 4, v2);
		QuiverMatrix res(4, 4);

		EXPECT_TRUE(exp.equals(m.enlarge_matrix(1, 1, res)));
	}

	TEST(QuiverMatrix, Enlarge2) {
		int v1[] = {0, 2, -2, -2, 0, 2, 2, -2, 0};
		int v2[] = {0, 2, -2, 0, 0, -2, 0, 2, 0, 0, 2, -2, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0};
		QuiverMatrix m(3, 3, v1);
		QuiverMatrix exp(5, 5, v2);
		QuiverMatrix res(5, 5);

		EXPECT_TRUE(exp.equals(m.enlarge_matrix(2, 2, res)));
	}

	TEST(QuiverMatrix, 5Mut) {
		int v1[] = {0, 0, -1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, -1, 1, -1, 0, 1, 0,
						-1, 0, -1, -1, 1, 0};
		int v2[] = {0, 0, -1, 1, 0, 0, 0, 0, 1, -1, 1, 0, 0, 0, -1, -1, -1, 0,
						0, 1, 0, 1, 1, -1, 0};
		QuiverMatrix m(5, 5, v1);
		QuiverMatrix exp(5, 5, v2);
		QuiverMatrix res(5, 5);

		EXPECT_TRUE(exp.equals(m.mutate(4, res)));
	}
	
	TEST(QuiverMatrix, NotInfinite) {
		int v1[] = {0, 2, -2, -2, 0, 2, 2, -2, 0};
		int v2[] = {0, 1, -1, 0, -0, 0, 1, 0, 2, -1, 0, 1, -2, 0, 0, 0};
		QuiverMatrix m(3, 3, v1);
		QuiverMatrix n(4, 4, v2);

		EXPECT_FALSE(m.is_infinite());
		EXPECT_FALSE(n.is_infinite());
	}
	
	TEST(QuiverMatrix, Infinite) {
		int v1[] = {0, 2, -2, -2, 0, 3, 2, -2, 0};
		int v2[] = {0, 1, -1, 0, -4, 0, 1, 0, 2, -1, 0, 1, -2, 0, 0, 0};
		QuiverMatrix m(3, 3, v1);
		QuiverMatrix n(4, 4, v2);

		EXPECT_TRUE(m.is_infinite());
		EXPECT_TRUE(n.is_infinite());
	}
}
