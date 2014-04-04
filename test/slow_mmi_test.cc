/*
 * slow_mmi_test.cc
 */


#include "gtest/gtest.h"
#include "mmi.h"
#include "dynkin.h"

namespace cluster {

	TEST(SlowMMI, NotInfinite) {
		QuiverMatrix m = dynkin::A5;
		
		EXPECT_FALSE(mmi::slow_mmi(m));
	}

	TEST(SlowMMI, MMI) {
		int v[] = {0, 1, 0, 0, -1, 0, 1, 1, 0, -1, 0, 1, 0, -1, -1, 0};
		QuiverMatrix m(4,4,v);

		EXPECT_TRUE(mmi::slow_mmi(m));
	}
	
	TEST(SlowMMI, NotMMI) {
		int v[] = {0, 1, 0, 0, 1, -1, 0, 1, 1, 1, 0, -1, 0, 1, 0, 0, -1, -1, 0, 0,
			-1, -1, 0, 0, 0};
		QuiverMatrix m(5,5,v);

		EXPECT_FALSE(mmi::slow_mmi(m));
	}

	TEST(SlowMMI, InfiniteNotMMI) {
		int v[] = {0, -1, 1, 0, 2, 1, 0, -1, 0, -2, -1, 1, 0, 1, 2, 0, 0, -1, 0, 0,
			-2, 2, -2, 0, 0};
		QuiverMatrix m(5,5,v);

		EXPECT_FALSE(mmi::slow_mmi(m));
	}
}

