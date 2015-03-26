/*
 * mutation_class_test.cc
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
