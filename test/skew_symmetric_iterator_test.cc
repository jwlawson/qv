/*
 * skew_symmetric_iterator_test.cc
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
#include "skew_symmetric_iterator.h"

#include "gtest/gtest.h"

#include "dynkin.h"

namespace cluster {

	TEST(SkewSymIter, Size3) {
		SkewSymmetricIterator iter(3);

		ASSERT_TRUE(iter.has_next());

		std::string s = "{ { 0 -2 -2 } { 2 0 -2 } { 2 2 0 } }";
		EquivQuiverMatrix m(s);
		EXPECT_TRUE(m.equals(*iter.next()));
		
		EXPECT_TRUE(iter.has_next());
	}

	TEST(SkewSymIter, A3) {
		SkewSymmetricIterator iter(3);

		bool has_A3 = false;
		IntMatrix a3 = dynkin::A3;

		while(iter.has_next() && !has_A3) {
			std::shared_ptr<EquivQuiverMatrix> n = iter.next();
			has_A3 = IntMatrix::are_equal(a3, *n);
		}
		EXPECT_TRUE(has_A3);
	}

}

