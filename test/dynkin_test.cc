/*
 * dynkin_test.cc
 */

#include "dynkin.h"
#include "gtest/gtest.h"

namespace cluster {

	TEST(Dynkin, GetFromMap) {

		QuiverMatrix a = dynkin::MAP.at("A3");
		
		EXPECT_TRUE(a.equals(dynkin::A3));
	}

}

