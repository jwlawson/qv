/**
 * mass_finite_check_test.cc
 */
#include "mass_finite_check.h"

#include "gtest/gtest.h"

#include "dynkin.h"

namespace cluster {

	TEST(MassFiniteCheck, Infinite) {
		int v[] = {0,1,0,0, -1,0,1,1, 0,-1,0,1, 0,-1,-1,0};
		EquivQuiverMatrix m(4,4,v);
		MassFiniteCheck chk;
		EXPECT_FALSE(chk.is_finite(m));
	}

	TEST(MassFiniteCheck, A3) {
		EquivQuiverMatrix m(dynkin::A3);
		MassFiniteCheck chk;

		EXPECT_TRUE(chk.is_finite(m));

		int v[] = {0, -1, 0, 1, 0, 1, 0, -1, 0 };
		EquivQuiverMatrix n(3,3,v);
		EXPECT_TRUE(chk.is_finite(n));
	}

	TEST(MassFiniteCheck, A6) {
		EquivQuiverMatrix m(dynkin::A6);
		MassFiniteCheck chk;

		EXPECT_TRUE(chk.is_finite(m));
	}

	TEST(MassFiniteCheck, Empty) {
		EquivQuiverMatrix m;
		MassFiniteCheck chk;

		EXPECT_TRUE(chk.is_finite(m));
	}

	TEST(MassFiniteCheck, A9) {
		EquivQuiverMatrix m(dynkin::A9);
		MassFiniteCheck chk;
		EXPECT_TRUE(chk.is_finite(m));
	}
}
