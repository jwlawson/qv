/*
 * cycle_test.cc
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
#include "cycle.h"

#include "gtest/gtest.h"

namespace cluster {

	TEST(Cycle, Contains) {
		std::vector<int> v = {0, 1, 2, 3};
		Cycle res(v);

		EXPECT_TRUE(res.contains(0));
		EXPECT_TRUE(res.contains(1));
		EXPECT_FALSE(res.contains(4));
		EXPECT_FALSE(res.contains(-1));
	}

	TEST(Cycle, SameEquals) {
		std::vector<int> v1 = {0, 1, 2};
		
		Cycle a(v1);
		Cycle b(v1);

		EXPECT_TRUE(a.equals(b));
	}

	TEST(Cycle, PermEquals) {
		std::vector<int> v1 = {0, 1, 2};
		std::vector<int> v2 = {1, 2, 0};

		Cycle a(v1);
		Cycle b(v2);

		EXPECT_TRUE(a.equals(b));
	}

	TEST(Cycle, NotEqual) {
		std::vector<int> v1 = {1, 2, 3};
		std::vector<int> v2 = {0, 1, 2, 3};

		Cycle a(v1);
		Cycle b(v2);

		EXPECT_FALSE(a.equals(b));
	}

	TEST(Cycle, NotEqualPerm) {
		std::vector<int> v1 = {1, 2, 3};
		std::vector<int> v2 = {2, 1, 3};

		Cycle a(v1);
		Cycle b(v2);

		EXPECT_FALSE(a.equals(b));
	}

	TEST(Cycle, PermutationEquals) {
		std::vector<int> v1 = {0, 1, 2};
		std::vector<int> v2 = {1, 2, 3};
		std::vector<int> map = {3, 0, 1, 2};

		Cycle a(v1);
		Cycle b(v2);

		bool res = a.equals(b, map);
		EXPECT_TRUE(res);
	}
}

