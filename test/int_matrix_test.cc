#include "gtest/gtest.h"
#include "int_matrix.h"
#include <iostream>

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
		IntMatrix m1(3, 3,v2 );
		IntMatrix m2(3, 3, v3);
		IntMatrix res1(3, 3);
		IntMatrix res2(3, 3);

		IntMatrix pa = p.mult_right(m1, res1);
		IntMatrix bp = p.mult_left(m2, res2);

		EXPECT_TRUE(pa.equals(bp));
		EXPECT_TRUE(bp.equals(pa));
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
		IntMatrix b(3, 3, v2 );
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
		IntMatrix res(3,3);

		EXPECT_TRUE(a.equals(a.mult_left(id, res)));
		EXPECT_TRUE(a.equals(id.mult_left(a, res)));
	}

	TEST(IntMatrix, Submatrix3x3a) {
		int v1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		int v2[] = {4, 5, 7, 8};
		IntMatrix m(3, 3, v1);
		IntMatrix exp(2, 2, v2);
		IntMatrix res(2, 2);

		EXPECT_TRUE(exp.equals(m.submatrix(0, 0, res)));
	}

	TEST(IntMatrix, Submatrix3x3b) {
		int v1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		int v2[] = {1, 2, 4, 5};
		IntMatrix m(3, 3, v1);
		IntMatrix exp(2, 2, v2);
		IntMatrix res(2, 2);
		EXPECT_TRUE(exp.equals(m.submatrix(2, 0, res)));
	}

	TEST(IntMatrix, Submatrix3x3c) {
		int v1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		int v2[] = {0, 1, 6, 7};
		IntMatrix m(3, 3, v1);
		IntMatrix exp(2, 2, v2);
		IntMatrix res(2, 2);

		EXPECT_TRUE(exp.equals( m.submatrix(1, 2, res)));
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
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		IntMatrix m(3, 3, v1);
		IntMatrix exp(6, 6, v2);
		IntMatrix res(6,6);
		m.enlarge_matrix(3, 3, res);
		EXPECT_TRUE(exp.equals(res));
	}
}
