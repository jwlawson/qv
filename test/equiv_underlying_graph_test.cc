/*
 * equiv_underlying_graph_test.cc
 */

#include "gtest/gtest.h"
#include "equiv_underlying_graph.h"
#include "dynkin.h"

namespace cluster {

	TEST(EquivUnderlying, Simple) {

		QuiverMatrix a3 = dynkin::A3;

		EquivUnderlyingGraph res(a3);

		int v[] = {0, 1, 0, 1, 0, 1, 0, 1, 0};
		IntMatrix exp(3, 3, v);

		EXPECT_TRUE(IntMatrix::are_equal(exp, res));

	}

	TEST(EquivUnderlying, Equivalence) {

		QuiverMatrix a4 = dynkin::A4;
		EquivUnderlyingGraph res(a4);

		int v[] = {0,0,1,1, 0,0,0,1, 1,0,0,0, 1,1,0,0};
		EquivQuiverMatrix exp(4, 4, v);

		EXPECT_TRUE(exp.equals(res));

	}

}
