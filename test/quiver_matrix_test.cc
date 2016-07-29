/*
 * quiver_matrix_test.cc
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

#include "quiver_matrix.h"

namespace cluster {

	TEST(QuiverMatrix, Construct) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		QuiverMatrix mat(3, 3, v1);

		EXPECT_EQ(0, mat.get(0, 0));
		EXPECT_EQ(1, mat.get(0, 1));
	}

	TEST(QuiverMatrix, Mutate) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, -1, 0, 1, 0, 1, 0, -1, 0};
		QuiverMatrix mat(3, 3, v1);
		QuiverMatrix exp1(3, 3, v2);
		QuiverMatrix res(3, 3);
		mat.mutate(0, res);

		EXPECT_TRUE(exp1.equals(res));
	}

	TEST(QuiverMatrix, Mutate2) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, -1, 1, 1, 0, -1, -1, 1, 0};
		QuiverMatrix mat(3, 3, v1);
		QuiverMatrix exp(3, 3, v2);
		QuiverMatrix res(3, 3);
		mat.mutate(1, res);
		EXPECT_TRUE(exp.equals(res));
	}

	TEST(QuiverMatrix, Markov) {
		int v1[] = {0, 2, -2, -2, 0, 2, 2, -2, 0};
		int v2[] = {0, -2, 2, 2, 0, -2, -2, 2, 0};
		QuiverMatrix mat(3, 3, v1);
		QuiverMatrix exp(3, 3, v2);
		QuiverMatrix res(3, 3);
		
		mat.mutate(0, res);
		EXPECT_TRUE(exp.equals(res));
		
		mat.mutate(1, res);
		EXPECT_TRUE(exp.equals(res));

		mat.mutate(2, res);
		EXPECT_TRUE(exp.equals(res));
	}

	TEST(QuiverMatrix, Enlarge) {
		int v1[] = {0, 2, -2, -2, 0, 2, 2, -2, 0};
		int v2[] = {0, 2, -2, 0, -2, 0, 2, 0, 2, -2, 0, 0, 0, 0, 0, 0};
		QuiverMatrix m(3, 3, v1);
		QuiverMatrix exp(4, 4, v2);
		QuiverMatrix res(4, 4);
		m.enlarge_matrix(res);

		EXPECT_TRUE(exp.equals(res));
	}

	TEST(QuiverMatrix, Enlarge2) {
		int v1[] = {0, 2, -2, -2, 0, 2, 2, -2, 0};
		int v2[] = {0, 2, -2, 0, 0, -2, 0, 2, 0, 0, 2, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		QuiverMatrix m(3, 3, v1);
		QuiverMatrix exp(5, 5, v2);
		QuiverMatrix res(5, 5);
		m.enlarge_matrix(res);
		EXPECT_TRUE(exp.equals(res));
	}

	TEST(QuiverMatrix, 5Mut) {
		int v1[] = {0, 0, -1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, -1, 1, -1, 0, 1, 0,
		            -1, 0, -1, -1, 1, 0
		           };
		int v2[] = {0, 0, -1, 1, 0, 0, 0, 0, 1, -1, 1, 0, 0, 0, -1, -1, -1, 0,
		            0, 1, 0, 1, 1, -1, 0
		           };
		QuiverMatrix m(5, 5, v1);
		QuiverMatrix exp(5, 5, v2);
		QuiverMatrix res(5, 5);
		m.mutate(4, res);
		EXPECT_TRUE(exp.equals(res));
	}

	TEST(QuiverMatrix, NotInfinite) {
		int v1[] = {0, 2, -2, -2, 0, 2, 2, -2, 0};
		int v2[] = {0, 1, -1, 0, -0, 0, 1, 0, 2, -1, 0, 1, -2, 0, 0, 0};
		QuiverMatrix m(3, 3, v1);
		QuiverMatrix n(4, 4, v2);

		EXPECT_FALSE(m.is_infinite());
		EXPECT_FALSE(n.is_infinite());
	}

	TEST(QuiverMatrix, Infinite) {
		int v1[] = {0, 2, -2, -2, 0, 3, 2, -2, 0};
		int v2[] = {0, 1, -1, 0, -4, 0, 1, 0, 2, -1, 0, 1, -2, 0, 0, 0};
		QuiverMatrix m(3, 3, v1);
		QuiverMatrix n(4, 4, v2);

		EXPECT_TRUE(m.is_infinite());
		EXPECT_TRUE(n.is_infinite());
	}

	TEST(QuiverMatrix, Assign) {
		int v1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
		int v2[] = {4, 5, 6, 7, 8, 9, 0, 1, 2};

		QuiverMatrix m(3, 3, v1);
		QuiverMatrix n(3, 3, v2);

		n = m;

		EXPECT_TRUE(m.equals(n));
		EXPECT_TRUE(n.equals(m));
	}

	TEST(QuiverMatrix, ZeroMutation) {
		int v[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		QuiverMatrix m(4, 4, v);
		QuiverMatrix exp(4, 4, v);
		QuiverMatrix res(4, 4);

		m.mutate(3, res);
		EXPECT_TRUE(res.equals(exp));
	}

	TEST(QuiverMatrix, LargeString) {
		std::string in {"{ "
		 "{  0 -1  0  0  0  0  0 -1  1  0  0  1 } "
		 "{  1  0  1  0  0  0  0  0  0 -1  0 -1 } "
		 "{  0 -1  0 -1  0  0  0  0  0  1  1  0 } "
		 "{  0  0  1  0  1  0  0  0  0  0 -2  0 } "
		 "{  0  0  0 -1  0 -1  0  0  0  1  1  0 } "
		 "{  0  0  0  0  1  0  1  0  0 -1  0 -1 } "
		 "{  0  0  0  0  0 -1  0 -1  1  0  0  1 } "
		 "{  1  0  0  0  0  0  1  0 -2  0  0  0 } "
		 "{ -1  0  0  0  0  0 -1  2  0  0  0  0 } "
		 "{  0  1 -1  0 -1  1  0  0  0  0  0  0 } "
		 "{  0  0 -1  2 -1  0  0  0  0  0  0  0 } "
		 "{ -1  1  0  0  0  1 -1  0  0  0  0  0 } "
		 "}"};
		QuiverMatrix q(in);
		EXPECT_EQ(12, q.num_cols());
		EXPECT_EQ(12, q.num_rows());
	}

}
