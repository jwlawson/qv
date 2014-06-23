/*
 * fast_mmi_test.cc
 *
 * Tests for the fast_mmi method.
 *
 * The tests *may* not pass every time as the method is not entirely 
 * deterministic, but this will only happen very rarely.
 */

#include "gtest/gtest.h"
#include "mmi.h"
#include "dynkin.h"

namespace cluster {

	TEST(FastMMI, NotInfinite) {
		QuiverMatrix m = dynkin::A5;
		
		EXPECT_FALSE(mmi::fast_mmi(m));
	}

	TEST(FastMMI, MMI) {
		int v[] = {0, 1, 0, 0, -1, 0, 1, 1, 0, -1, 0, 1, 0, -1, -1, 0};
		QuiverMatrix m(4,4,v);

		EXPECT_TRUE(mmi::fast_mmi(m));
	}
	
	TEST(FastMMI, NotMMI) {
		int v[] = {0, 1, 0, 0, 1, -1, 0, 1, 1, 1, 0, -1, 0, 1, 0, 0, -1, -1, 0, 0,
			-1, -1, 0, 0, 0};
		QuiverMatrix m(5,5,v);

		EXPECT_FALSE(mmi::fast_mmi(m));
	}

	TEST(FastMMI, InfiniteNotMMI) {
		int v[] = {0, -1, 1, 0, 2, 1, 0, -1, 0, -2, -1, 1, 0, 1, 2, 0, 0, -1, 0, 0,
			-2, 2, -2, 0, 0};
		QuiverMatrix m(5,5,v);

		EXPECT_FALSE(mmi::fast_mmi(m));
	}

	TEST(FastMMI, EmptySubmatrices) {
		int v[] = {0, 1, 1, 1, 1, 1, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0};
		QuiverMatrix m(6, 6, v);

		EXPECT_TRUE(mmi::fast_mmi(m));
	}
}

