/*
 * slow_mmi_test.cc
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
#include "mmi.h"
#include "dynkin.h"
#include <string>

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

	TEST(SlowMMI, NonHyperbolic) {
		std::string s = "{ { 0 -1 1 0 1 } { 1 0 -1 0 -1 } { -1 1 0 1 0 } { 0 0 -1 0 1 } { -1 1 0 -1 0 } }";
		QuiverMatrix a(s);

		EXPECT_TRUE(mmi::slow_mmi(a));
	}

	TEST(SlowMMI, 4x4) {
		std::string s = "{ { 0 -1 0 1 } { 1 0 1 1 } { 0 -1 0 1 } { -1 -1 -1 0 } }";
		QuiverMatrix a(s);

		EXPECT_TRUE(mmi::slow_mmi(a));
	}

	TEST(SlowMMI, IsMMI) {
		std::string s = "{ { 0 1 0 -1 } { -1 0 -1 -1 } { 0 1 0 -1 } { 1 1 1 0 } }";
		QuiverMatrix a(s);

		EXPECT_TRUE(mmi::slow_mmi(a));
	}

	TEST(SlowMMI, EmptySubmatrices) {
		int v[] = {0, 1, 1, 1, 1, 1, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0};
		QuiverMatrix m(6, 6, v);

		EXPECT_TRUE(mmi::slow_mmi(m));
	}
}

