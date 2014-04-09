/*
 * underlying_graph_test.cc
 */

#include "gtest/gtest.h"
#include "underlying_graph.h"
#include "dynkin.h"

namespace cluster {

	TEST(UnderlyingGraph, IntMatrix) {
		QuiverMatrix m = dynkin::A3;
		UnderlyingGraph res(m);
		int v[] = {0,1,0,1,0,1,0,1,0};
		IntMatrix exp(3,3,v);

		EXPECT_TRUE(exp.equals(res));
	}

}

