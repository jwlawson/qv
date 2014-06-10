/*
 * mutation_class_test.cc
 */
#include "mutation_class.h"

#include "gtest/gtest.h"

#include "dynkin.h"

namespace cluster {

	TEST(MutationClass, A3) {
		QuiverMatrix mat = dynkin::A3;
		MutationClass c(mat);

		EXPECT_TRUE(c.is_finite());
		EXPECT_EQ(4, c.size());
		EXPECT_TRUE(c.contains(std::make_shared<EquivQuiverMatrix>(mat)));
	}

	TEST(MutationClass, Infinite) {
		int v[] = {0,1,0,0, -1,0,1,1, 0,-1,0,1, 0,-1,-1,0};
		EquivQuiverMatrix m(4,4,v);
		MutationClass c(m);

		EXPECT_FALSE(c.is_finite());
	}
}
