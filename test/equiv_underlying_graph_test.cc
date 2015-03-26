/*
 * equiv_underlying_graph_test.cc
 * Copyright 2014-2015 John Lawson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
