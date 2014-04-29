/*
 * cysle_info_test.cc
 */
#include "cycle_info.h"

#include "gtest/gtest.h"

namespace cluster {

	TEST(CycleInfo, SameEqual) {
		int v[] = {0, 1, -1, -1, 0, 1, 1, -1, 0};
		QuiverMatrix m(3, 3, v);
		QuiverMatrix n(3, 3, v);

		CycleInfo c1(m);
		CycleInfo c2(n);

		EXPECT_TRUE(c1.equals(c2));
	}

	TEST(CycleInfo, PermEqual) {
		int v[] = {0, -1, 1, 1, 0, -1, -1, 1, 0};
		int w[] = {0, 1, -1, -1, 0, 1, 1, -1, 0};

		QuiverMatrix m(3, 3, v);
		QuiverMatrix n(3, 3, w);

		CycleInfo c1(m);
		CycleInfo c2(n);

		EXPECT_TRUE(c1.equals(c2));
	}

	TEST(CycleInfo, ExtensionEqual) {
		int v[] = {0, 1, 0, 0, -1, 0, 1, -1, 0, -1, 0, 1, 0, 1, -1, 0};
		int w[] = {0, 1, -1, 0, -1, 0, 1, 0, 1, -1, 0, 1, 0, 0, -1, 0};

		QuiverMatrix m(4, 4, v);
		QuiverMatrix n(4, 4, w);

		CycleInfo c1(m);
		CycleInfo c2(n);

		EXPECT_TRUE(c1.equals(c2));
	}

	TEST(CycleInfo, NoCycles) {
		int v[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int w[] = {0, 1, 0, 0, 0, 1,
							 -1, 0, 1, 0, 0, 0,
							 0, -1, 0, 1, 0, 0,
							 0, 0, -1, 0, -1, 0,
							 0, 0, 0, 1, 0, -1,
							 -1, 0, 0, 0, 1, 0};

		QuiverMatrix m(3, 3, v);
		QuiverMatrix n(6, 6, w);

		CycleInfo c1(m);
		CycleInfo c2(n);

		EXPECT_TRUE(c1.equals(c2));
	}

	TEST(CycleInfo, DoubleNotSingle) {
		int v[] = { 0, 2, -1, -2, 0, 1, 1, -1, 0};
		int w[] = {0, 1, -1, -1, 0, 1, 1, -1, 0};

		QuiverMatrix m(3, 3, v);
		QuiverMatrix n(3, 3, w);

		CycleInfo c1(m);
		CycleInfo c2(n);

		EXPECT_FALSE(c1.equals(c2));
	}

	TEST(CycleInfo, DoubleExtension) {
		int v[] = { 0, 2, -1, -2, 0, 1, 1, -1, 0};
		int w[] = {0, 1, 0, 0, -1, 0, 2, -1, 0, -2, 0, 1, 0, 1, -1, 0};

		QuiverMatrix m(3, 3, v);
		QuiverMatrix n(4, 4, w);

		CycleInfo c1(m);
		CycleInfo c2(n);

		EXPECT_TRUE(c1.equals(c2));
	}

	TEST(CycleInfo, NotEqual) {
		int v[] = {0, -1, 1, 1, 0, -1, -1, 1, 0};
		int w[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};

		QuiverMatrix m(3, 3, v);
		QuiverMatrix n(3, 3, w);

		CycleInfo c1(m);
		CycleInfo c2(n);

		EXPECT_FALSE(c1.equals(c2));
	}
}

