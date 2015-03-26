/*
 * stream_iterator_test.cc
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
#include "stream_iterator.h"

#include <fstream>
#include <sstream>
#include <unordered_set>

#include "gtest/gtest.h"

#include "quiver_matrix.h"
#include "equiv_quiver_matrix.h"

namespace cluster {

	TEST(StreamIterator, First) {
		std::stringstream ss;
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix mat(3,3,v);
		ss << mat;

		StreamIterator<QuiverMatrix> iter(ss);

		ASSERT_TRUE(iter.has_next());
		EXPECT_TRUE(mat.equals(*(iter.next())));
		EXPECT_FALSE(iter.has_next());
	}

	TEST(StreamIterator, Empty) {
		std::ifstream file;
		file.open("test/empty");
		ASSERT_TRUE(file.is_open());

		StreamIterator<QuiverMatrix> iter(file);

		EXPECT_FALSE(iter.has_next());
	}

	/*
	 * Adding 30,000 matrices to the set takes 5 hours.
	 *
	TEST(StreamIterator, Huge) {
		std::ifstream file;
		file.open("/home/grads/njcz19/result/finite/9_fin");
		ASSERT_TRUE(file.is_open());

		StreamIterator<EquivQuiverMatrix> iter(file);
		std::unordered_set<std::shared_ptr<EquivQuiverMatrix>> set;

		EXPECT_TRUE(iter.has_next());
		int count = 0;
		while(iter.has_next()) {
			set.insert(iter.next());
			count++;
		}
		EXPECT_EQ(count, 30167);
	}
	*/
}

