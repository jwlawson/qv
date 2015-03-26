/*
 * mutation_class_size_test.cc
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
#include "mutation_class_size.h"
#include "dynkin.h"

namespace cluster {

	TEST(MutationClassSize, A3) {
		QuiverMatrix m = dynkin::A3;
		int exp = 14;
		int result = classsize::Size(m);

		EXPECT_EQ(exp, result);
	}

	TEST(MutationClassSize, A4) {
		QuiverMatrix m = dynkin::A4;
		int exp = 144;
		int result = classsize::Size(m);

		EXPECT_EQ(exp, result);
	}

	TEST(MutationClassSize, D4) {
		QuiverMatrix m = dynkin::D4;
		int exp = 50;
		int result = classsize::Size(m);

		EXPECT_EQ(exp, result);
	}

	TEST(MutationClassSize, D5) {
		QuiverMatrix m = dynkin::D5;
		int exp = 2184;
		int res = classsize::Size(m);

		EXPECT_EQ(exp, res);
	}

	TEST(MutationClassSize, E6) {
		QuiverMatrix m = dynkin::E6;
		int exp = 42840;
		int res = classsize::Size(m);

		EXPECT_EQ(exp, res);
	}

	TEST(MutationClassSize, Infinite) {
		int v[] = {0, 1, 0, 0, -1, 0, 1, 1, 0, -1, 0, 1, 0, -1, -1, 0};
		QuiverMatrix m(4, 4, v);
		int exp = -1;
		int res = classsize::Size(m);

		EXPECT_EQ(exp, res);
	}
	/* A7 takes 23 seconds to calculate on my machine. Not including this test
	 * as I prefer the test to be quicker than that.
	 */
}


