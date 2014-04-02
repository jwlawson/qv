/*
 * equiv_mutation_class_size_test.cc
 */

#include "gtest/gtest.h"
#include "equiv_mutation_class_size.h"
#include "dynkin.h"

namespace cluster {

	TEST(EquivMutationClassSize, A3) {
		EquivQuiverMatrix m(dynkin::A3);
		int exp = 4;
		int res = equivsize::Size(m);

		EXPECT_EQ(exp, res);
	}


	TEST(EquivMutationClassSize, A5) {
		EquivQuiverMatrix m(dynkin::A5);
		int exp = 19;
		int res = equivsize::Size(m);

		EXPECT_EQ(exp, res);
	}


	TEST(EquivMutationClassSize, D6) {
		EquivQuiverMatrix m(dynkin::D6);
		int exp = 80;
		int res = equivsize::Size(m);

		EXPECT_EQ(exp, res);
	}
}
