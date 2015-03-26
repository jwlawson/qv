/*
 * int_matrix_test.cc
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
#include "int_matrix.h"
#include <functional>
#include <string>

namespace cluster {

	TEST(IntMatrix, Equals) {
		int v1[] = {0, 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 4, 0, 2, 0, 0};
		int v2[] = {0, 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 4, 0, 2, 0, 0};
		IntMatrix m1(4, 4, v1);
		IntMatrix m2(4, 4, v2);

		EXPECT_TRUE(m1.equals(m1));
		EXPECT_TRUE(m1.equals(m2));
	}

	TEST(IntMatrix, MultEquals) {
		int v1[] = {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0};
		int v2[] = {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4};
		int v3[] = {3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 2};

		IntMatrix p(4, 4, v1);
		IntMatrix a(4, 4, v2);
		IntMatrix b(4, 4, v3);

		IntMatrix pa(4, 4);
		IntMatrix bp(4, 4);

		a.mult_left(p, pa);
		b.mult_right(p, bp);

		EXPECT_TRUE(pa.equals(bp));
		EXPECT_TRUE(bp.equals(pa));
	}

	TEST(IntMatrix, 3Equals) {
		int v1[] = {0, 1, 0, 0, 0, 1, 1, 0, 0};
		int v2[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v3[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		IntMatrix p(3, 3, v1);
		IntMatrix m1(3, 3, v2);
		IntMatrix m2(3, 3, v3);
		IntMatrix res1(3, 3);
		IntMatrix res2(3, 3);

		p.mult_right(m1, res1);
		p.mult_left(m2, res2);

		EXPECT_TRUE(res2.equals(res1));
		EXPECT_TRUE(res1.equals(res2));
	}

	TEST(IntMatrix, Multiplication) {
		int v1[] = {1, 2, 3, 4};
		int v2[] = {5, 6, 7, 8};
		int v3[] = {19, 22, 43, 50};
		IntMatrix a(2, 2, v1);
		IntMatrix b(2, 2, v2);
		IntMatrix exp(2, 2, v3);
		IntMatrix res(2, 2);

		a.mult_right(b, res);

		EXPECT_TRUE(exp.equals(res));
	}

	TEST(IntMatrix, Multiplication3x3) {
		int v1[] = {1, 2, 3, 1, 3, 2, 5, 4, 3};
		int v2[] = {5, 5, 5, 2, 3, 4, 7, 6, 4};
		int v3[] = {30, 29, 25, 25, 26, 25, 54, 55, 53};
		IntMatrix a(3, 3, v1);
		IntMatrix b(3, 3, v2);
		IntMatrix exp(3, 3, v3);
		IntMatrix res(3, 3);
		a.mult_right(b, res);

		EXPECT_TRUE(exp.equals(res));
	}

	TEST(IntMatrix, Identity) {
		int v1[] = {0, 0, -1, 0, 0, 1, 1, -1, 0};
		int v2[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
		IntMatrix a(3, 3, v1);
		IntMatrix id(3, 3, v2);
		IntMatrix res(3, 3);

		a.mult_left(id, res);
		EXPECT_TRUE(a.equals(res));

		id.mult_left(a, res);
		EXPECT_TRUE(a.equals(res));
	}

	TEST(IntMatrix, Submatrix3x3a) {
		int v1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		int v2[] = {4, 5, 7, 8};
		IntMatrix m(3, 3, v1);
		IntMatrix exp(2, 2, v2);
		IntMatrix res(2, 2);
		m.submatrix(0, 0, res);

		EXPECT_TRUE(exp.equals(res));
	}

	TEST(IntMatrix, Submatrix3x3b) {
		int v1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		int v2[] = {1, 2, 4, 5};
		IntMatrix m(3, 3, v1);
		IntMatrix exp(2, 2, v2);
		IntMatrix res(2, 2);
		m.submatrix(2, 0, res);
		EXPECT_TRUE(exp.equals(res));
	}

	TEST(IntMatrix, Submatrix3x3c) {
		int v1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		int v2[] = {0, 1, 6, 7};
		IntMatrix m(3, 3, v1);
		IntMatrix exp(2, 2, v2);
		IntMatrix res(2, 2);
		m.submatrix(1, 2, res);

		EXPECT_TRUE(exp.equals(res));
	}

	TEST(IntMatrix, ZeroRow) {
		int v1[] = {1, 2, 3, 0, 0, 0, 1, 2, 3};
		IntMatrix m(3, 3, v1);
		EXPECT_EQ(1, m.zero_row());
	}

	TEST(IntMatrix, FirstZeroRow) {
		int v1[] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
		IntMatrix m(4, 4, v1);
		EXPECT_EQ(0, m.zero_row());
	}

	TEST(IntMatrix, LastZeroRow) {
		int v1[] = {0, 1, 1, 0, -1, 0, 1, 0, -1, -1, 0, 0, 0, 0, 0, 0};
		IntMatrix m(4, 4, v1);
		EXPECT_EQ(3, m.zero_row());
	}

	TEST(IntMatrix, NonZeroRow) {
		int v1[] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
		IntMatrix m(4, 4, v1);
		EXPECT_EQ(-1, m.zero_row());
	}

	TEST(IntMatrix, Enlarge3To6) {
		int v1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
		int v2[] = {1, 2, 3, 0, 0, 0, 4, 5, 6, 0, 0, 0, 7, 8, 9, 0, 0, 0, 0, 0, 0,
		            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		           };
		IntMatrix m(3, 3, v1);
		IntMatrix exp(6, 6, v2);
		IntMatrix res(6, 6);
		m.enlarge_matrix(res);
		EXPECT_TRUE(exp.equals(res));
	}

	TEST(IntMatrix, StlEqualOverride) {
		int v1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
		IntMatrix m(3, 3, v1);
		IntMatrix n(3, 3, v1);
		std::equal_to<IntMatrix> eq;

		EXPECT_TRUE(eq(m, n));
	}

	TEST(IntMatrix, Assign) {
		int v1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
		int v2[] = {4, 5, 6, 7, 8, 9, 0, 1, 2};

		IntMatrix m(3, 3, v1);
		IntMatrix n(3, 3, v2);

		n = m;

		EXPECT_TRUE(m.equals(n));
		EXPECT_TRUE(n.equals(m));
	}

	TEST(IntMatrix, PermRows) {
		int v1[] = {1,2,3,4,5,6,7,8,9};
		int v2[] = {4,5,6,1,2,3,7,8,9};

		IntMatrix a(3,3,v1);
		IntMatrix exp(3,3,v2);

		IntMatrix res(3,3);
		std::vector<int> vec = {1, 0, 2};
		a.permute_rows(vec, res);

		EXPECT_TRUE(exp.equals(res));
	}

	TEST(IntMatrix, PermCols) {
		int v1[] = {1,2,3,4,5,6,7,8,9};
		int v2[] = {2,3,1,5,6,4,8,9,7};

		IntMatrix a(3,3,v1);
		IntMatrix exp(3,3,v2);

		IntMatrix res(3,3);
		std::vector<int> vec = {2, 0, 1};
		a.permute_columns(vec, res);

		EXPECT_TRUE(exp.equals(res));
	}

	TEST(IntMatrix, String) {
		std::string str = "{ { 0 1 2 } { 3 4 5 } { 6 7 8 } }";
		IntMatrix a(str);

		int v[] = {0,1,2,3,4,5,6,7,8};
		IntMatrix exp(3,3,v);

		EXPECT_TRUE(exp.equals(a));
	}
	
	TEST(IntMatrix, StringHuge) {
		std::string str = "{ { 0 1 0 0 0 0 0 0 0 0 } "
												"{ -1 0 1 0 0 0 0 0 0 0 } "
												"{ 0 -1 0 1 0 0 0 0 0 0 } "
												"{ 0 0 -1 0 1 0 0 0 0 0 } "
												"{ 0 0 0 -1 0 1 0 0 0 0 } "
												"{ 0 0 0 0 -1 0 1 0 0 -1 } "
												"{ 0 0 0 0 0 -1 0 1 0 0 } "
												"{ 0 0 0 0 0 0 -1 0 1 0 } "
												"{ 0 0 0 0 0 0 0 -1 0 1 } "
												"{ 0 0 0 0 0 1 0 0 -1 0 } }";
		IntMatrix a(str);

		int v[] = {0,1,0,0,0,0,0,0,0,0,
							-1,0,1,0,0,0,0,0,0,0,
							0,-1,0,1,0,0,0,0,0,0,
							0,0,-1,0,1,0,0,0,0,0,
							0,0,0,-1,0,1,0,0,0,0,
							0,0,0,0,-1,0,1,0,0,-1,
							0,0,0,0,0,-1,0,1,0,0,
							0,0,0,0,0,0,-1,0,1,0,
							0,0,0,0,0,0,0,-1,0,1,
							0,0,0,0,0,1,0,0,-1,0};
		IntMatrix exp(10, 10, v);

		EXPECT_TRUE(exp.equals(a));
	}

	TEST(IntMatrix, SubmatrixVector) {

		std::string str = "{ { 0 1 2 } { 3 4 5 } { 6 7 8 } }";
		IntMatrix a(str);

		std::vector<int> row = {0, 1};
		std::vector<int> col = {2};

		IntMatrix res(2, 1);
		a.submatrix(std::move(row), std::move(col), res);

		std::string exp_str = "{ { 2 } { 5 } }";
		IntMatrix exp(exp_str);

		EXPECT_TRUE(exp.equals(res));
	}

	TEST(IntMatrix, SubmatrixVectorBig) {
		std::string str = "{ {  1  2  3  4  5  6 } "
												"{  7  8  9 10 11 12 } "
												"{ 13 14 15 16 17 18 } "
												"{ 19 20 21 22 23 24 } "
												"{ 25 26 27 28 29 30 } "
												"{ 31 32 33 34 35 36 } }";
		IntMatrix a(str);

		std::vector<int> row = {0, 4, 5};
		std::vector<int> col = {1, 3, 4, 5};

		IntMatrix res(3, 4);
		a.submatrix(std::move(row), std::move(col), res);

		std::string exp_str = "{ {  2  4  5  6 } "
														"{ 26 28 29 30 } "
														"{ 32 34 35 36 } }";
		IntMatrix exp(exp_str);

		EXPECT_TRUE(exp.equals(res));
	}
	
	TEST(IntMatrix, SubBug) {
		std::string str = "{ { 0 1 -1 0 } { -1 0 1 0 } { 1 -1 0 1 } { 0 0 -1 0 } }";
		IntMatrix a(str);

		std::vector<int> map = {0, 1, 2};
		IntMatrix res(3, 3);
		a.submatrix(map, map, res);

		std::string exp_str = "{ { 0 1 -1 } { -1 0 1 } { 1 -1 0 } }";
		IntMatrix exp(exp_str);

		EXPECT_TRUE(exp.equals(res));
	}

}
