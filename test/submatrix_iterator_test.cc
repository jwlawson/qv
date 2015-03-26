/*
 * submatrix_iterator_test.cc
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
#include "submatrix_iterator.h"

#include "gtest/gtest.h"

#include "quiver_matrix.h"

namespace cluster {
	
	TEST(SubmatrixIter, First) {
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix m(3,3,v);

		int w[] = {5,6,8,9};
		QuiverMatrix exp(2,2,w);

		SubmatrixIterator<QuiverMatrix> iter(m);

		EXPECT_TRUE(exp.equals(*(iter.next())));
	}

	TEST(SubmatrixIter, Second) {
		int v[] = {1,2,3,4,5,6,7,8,9};
		QuiverMatrix m(3,3,v);

		int e[] = {1,3,7,9};
		QuiverMatrix exp(2,2,e);

		SubmatrixIterator<QuiverMatrix> iter(m);
		iter.next();

		EXPECT_TRUE(exp.equals(*(iter.next())));
	}

	TEST( SubmatrixIter, Degenerate) {
		int v[] = {1,4,5,9,0,0,2,0,0};
		QuiverMatrix m(3,3,v);

		QuiverMatrix exp;

		SubmatrixIterator<QuiverMatrix> iter(m);

		EXPECT_TRUE(exp.equals(*(iter.next())));
	}
}
